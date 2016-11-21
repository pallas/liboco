#include "file_descriptor.h"

#include <lace/try.h>

#include <cassert>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>

#include <sys/stat.h>
#include <sys/socket.h>

namespace {

  int get_flags(int fd, bool status) {
    return TRY(fcntl, fd, status ? F_GETFL : F_GETFD, 0);
  }

  void set_flags(int fd, int add, bool status) {
    int flags = get_flags(fd, status);
    if (0 == (flags & add))
      TRY(fcntl, fd, status ? F_SETFL : F_SETFD, flags | add);
    assert(0 != (get_flags(fd, status) & add));
  }

  void
  unset_flags(int fd, int del, bool status) {
    int flags = get_flags(fd, status);
    if (flags & del)
      TRY(fcntl, fd, status ? F_SETFL : F_SETFL, flags & ~del);
    assert(0 == (get_flags(fd, status) & del));
  }

  template <typename T>
  T get_socket_option(int fd, int option) {
    T t; socklen_t l = sizeof t;
    assert(file_descriptor::is_socket(fd));
    TRY(getsockopt, fd, SOL_SOCKET, option, &t, &l);
    assert(l == sizeof t);
    return t;
  }

  template <typename T>
  void set_socket_option(int fd, int option, const T & value) {
    assert(file_descriptor::is_socket(fd));
    TRY(setsockopt, fd, SOL_SOCKET, option, &value, sizeof value);
  }

}

file_descriptor::file_descriptor()
  : fd(-1)
{ assert(!valid()); }

file_descriptor::file_descriptor(int i)
  : fd(i)
{ assert(valid()); }

file_descriptor::file_descriptor(const file_descriptor & o)
  : fd(o.valid() ? file_descriptor::dup(o.fd) : -1)
{ }

file_descriptor::~file_descriptor() {
  if (valid())
    TRY(close, fd);
}

file_descriptor &
file_descriptor::operator=(file_descriptor o) {
  std::swap(fd, o.fd);
  return *this;
}

bool
file_descriptor::valid() const {
  return 0 <= fd;
}

file_descriptor &
file_descriptor::capture(int i) {
  if (valid())
    TRY(close, fd);
  fd = i;
  assert(valid());
  return *this;
}

int
file_descriptor::release() {
  int r = -1;
  std::swap(fd, r);
  return r;
}

file_descriptor::operator int () const {
  assert(valid());
  return fd;
}

file_descriptor &
file_descriptor::block() {
  unset_flags(fd, O_NONBLOCK, true);
  return *this;
}

file_descriptor &
file_descriptor::unblock() {
  set_flags(fd, O_NONBLOCK, true);
  return *this;
}

file_descriptor &
file_descriptor::open_on_exec() {
  unset_flags(fd, FD_CLOEXEC, false);
  return *this;
}

file_descriptor &
file_descriptor::close_on_exec() {
  set_flags(fd, FD_CLOEXEC, false);
  return *this;
}

int
file_descriptor::dup(int ofd) {
  assert(0 <= ofd);
  int flags = get_flags(ofd, false);
  bool cloexec = FD_CLOEXEC & flags;
  int nfd = TRY_ERR(EINVAL, fcntl, ofd, cloexec ? F_DUPFD_CLOEXEC : F_DUPFD);
  if (nfd < 0)
    nfd = TRY(dup, ofd);
  if (flags)
    set_flags(nfd, flags, false);
  return nfd;
}

bool
file_descriptor::is_fifo(int fd) {
  assert(0 <= fd);
  struct stat s;
  TRY(fstat, fd, &s);
  return S_ISFIFO(s.st_mode);
}

bool
file_descriptor::is_socket(int fd) {
  assert(0 <= fd);
  struct stat s;
  TRY(fstat, fd, &s);
  return S_ISSOCK(s.st_mode);
}

int
file_descriptor::send_buffer(int fd) {
  return get_socket_option<int>(fd, SO_SNDBUF);
}

int
file_descriptor::receive_buffer(int fd) {
  return get_socket_option<int>(fd, SO_RCVBUF);
}

void
file_descriptor::keepalive(int fd) {
  return set_socket_option(fd, SO_KEEPALIVE, 1);
}

void
file_descriptor::reuseaddr(int fd) {
  return set_socket_option(fd, SO_REUSEADDR, 1);
}

//
