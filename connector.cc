#include "connector.h"

#include <lace/try.h>

#include <sys/socket.h>

namespace {

  static const int flags = SOCK_NONBLOCK | SOCK_CLOEXEC;

}

simple_connector::simple_connector(const socket_address & addr)
  : simple_task(TRY(socket, addr.protocol(), SOCK_STREAM|flags, 0))
  , a(addr)
{ }

void
simple_connector::operator()() {
  while (!dead()) {
    if (!connect(t.fd(), &a.cast(), a.length())) {
      on_connect(t.fd());
      return;
    }

    switch (errno) {
    case EAGAIN:
    case EALREADY:
    case EINPROGRESS:
      core::wait_for_write(t);
      continue;

    case EINTR:
      continue;

    default:
      on_error();
      continue;
    }
  }
}

void
simple_connector::on_error() {
  kill();
}

//
