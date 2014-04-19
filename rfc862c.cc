#include "try.h"
#include "core.h"

#include "task.h"
#include "trigger.h"
#include "connector.h"
#include "inet_address.h"
#include "file_descriptor.h"

#include <cassert>
#include <cstdlib>
#include <climits>
#include <iostream>

#include <netdb.h>

class splice : public task {
public:
  splice(int from, int into)
    : r(from)
    , w(into)
  { }

  ~splice() {
    for (int i = 0 ; i < 1 ; ++i) {
      if (file_descriptor::is_socket(r.fd()))
        core::shutdown(r, true, false, i);

      if (file_descriptor::is_socket(w.fd()))
        core::shutdown(w, false, true, i);
    }
  }

  void operator()() {
    int buf_size = PIPE_BUF;
    if (file_descriptor::is_socket(r.fd()))
      buf_size = file_descriptor::receive_buffer(r.fd());

    char buf[buf_size];

    while (size_t rx = core::read(r, buf, 1, sizeof buf)) {
      size_t wx = core::write(w, buf, rx, rx);
      if (wx < rx)
        break;
    }
  }

protected:
  trigger r, w;
};

class stdin_to_echo : public splice {
public:
  stdin_to_echo(int fd)
    : splice(fileno(stdin), fd)
  { }
};

class echo_to_stdout : public splice {
public:
  echo_to_stdout(int fd)
    : splice(fd, fileno(stdout))
  { }

  ~echo_to_stdout() {
    if (file_descriptor::is_socket(r.fd()))
      core::shutdown(r, true, true);
  }
};

int main(int argc, char *argv[]) {
  using namespace std;

  file_descriptor(fileno(stdin)).unblock().close_on_exec().release();
  file_descriptor(fileno(stdout)).unblock().close_on_exec().release();

  for (int i = 1 ; i < argc ; ++i) {
    struct addrinfo hints;
    struct addrinfo *res = NULL;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (int r = getaddrinfo(argv[i], "echo", &hints, &res)) {
      cerr << argv[i] << ": " << gai_strerror(r) << endl;
      continue;
    }

    for (struct addrinfo *re = res ; re ; re = re->ai_next)
    try {
      any_address addr(re->ai_addr, re->ai_addrlen);
      connector<echo_to_stdout, stdin_to_echo> c(addr);

      core::instance()
        .add(c)
        .react();

      if (!c.dead()) {
        freeaddrinfo(res);
        return EXIT_SUCCESS;
      }

    } catch (const std::runtime_error & e) {
      cerr << argv[i] << ": " << e.what() << endl;
    }

    freeaddrinfo(res);
  }

  return EXIT_FAILURE;
}

//
