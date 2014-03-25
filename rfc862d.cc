// rfc862 tcp echo daemon
#include "try.h"
#include "core.h"

#include "killer.h"
#include "acceptor.h"
#include "signaler.h"
#include "signal_set.h"
#include "simple_task.h"
#include "inet_address.h"
#include "socket_address.h"
#include "resource_limit.h"
#include "file_descriptor.h"

#include <cassert>
#include <cstdlib>
#include <unistd.h>

class echoer : public simple_task {
public:
  echoer(int fd)
    : simple_task(fd)
  { assert(file_descriptor::is_socket(t.fd())); }

  ~echoer() {
    core::shutdown(t, true, true);
  }

  void operator()() {
    int buf_size = file_descriptor::receive_buffer(t.fd());

    char buf[buf_size];

    while (size_t rx = core::read(t, buf, sizeof buf, sizeof buf)) {
      size_t wx = core::write(t, buf, rx, rx);
      if (wx < rx)
        break;
      core::instance().schedule();
    }
  }
};


class signal_killer : public signaler, public killer {
public:
  signal_killer(const signal_set & ss)
    : signaler(ss)
  { }

  void on_signal(const info_t &) {
    killer::killall();
  }
};

int main(int argc, char *argv[]) {
  core & c = core::instance();

  resource_limit::hog<RLIMIT_NOFILE>();

  TRY(close, fileno(stdin));
  TRY(close, fileno(stdout));

  signal_killer sk(signal_set().add(SIGINT).block());
  c.eye(sk);

  acceptor<echoer> *a = new acceptor<echoer>(inet_address(7));
  sk.target(*a);

  c.own(a).react();

  return EXIT_SUCCESS;
}

//
