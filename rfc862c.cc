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

#include "sys/stat.h"
#include "signal_set.h"

int main(int argc, char *argv[]) {
  inet_address addr(7, "127.0.0.1");

  file_descriptor(fileno(stdin)).unblock().close_on_exec().release();
  file_descriptor(fileno(stdout)).unblock().close_on_exec().release();

  connector<echo_to_stdout, stdin_to_echo> c(addr);
  core::instance()
    .add(c)
    .react();

  return c.dead() ? EXIT_FAILURE : EXIT_SUCCESS;
}

//
