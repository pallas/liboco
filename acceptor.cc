#include "acceptor.h"

#include "try.h"

#include <climits>
#include <sys/socket.h>

namespace {

  static const int flags = SOCK_NONBLOCK | SOCK_CLOEXEC;

}

simple_acceptor::simple_acceptor(const socket_address & addr)
  : simple_task(TRY(socket, addr.protocol(), SOCK_STREAM|flags, 0))
{
  file_descriptor::reuseaddr(t.fd());
  TRY(bind, t.fd(), &addr.cast(), addr.length());
  TRY(listen, t.fd(), INT_MAX);
}

void
simple_acceptor::operator()() {
  while (!dead()) {
    any_address addr;
    socklen_t len = addr.length();

    int fd = accept4(t.fd(), &addr.cast(), &len, flags);
    if (0 <= fd) {
      on_accept(fd, addr);
      continue;
    }

    switch (errno) {
    case EAGAIN:
      core::wait_for_read(t);
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
simple_acceptor::on_error() {
  kill();
}

//
