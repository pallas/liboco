#include "socket_address.h"

#include <cassert>
#include <cstring>

int
socket_address::protocol() const {
  switch (family()) {
  case AF_LOCAL:
    return PF_LOCAL;

  case AF_INET:
    return PF_INET;

  case AF_INET6:
    return PF_INET6;

  default:
    return PF_UNSPEC;
  }
}

int socket_address::family() const { return cast().sa_family; }

bool socket_address::is_inet() const { return AF_INET == family(); }
bool socket_address::is_inet6() const { return AF_INET6 == family(); }
bool socket_address::is_local() const { return AF_LOCAL == family(); }

any_address::any_address() { }

any_address::any_address(const socket_address & o) {
  assert(o.length() <= sizeof sa);
  memcpy(&sa, &o.cast(), o.length());
}

any_address::any_address(const struct sockaddr *addr, socklen_t addrlen) {
  assert(addrlen <= sizeof sa);
  memcpy(&sa, addr, addrlen);
}

any_address &
any_address::operator=(const socket_address & o) {
  assert(o.length() <= sizeof sa);
  memcpy(&sa, &o.cast(), o.length());
  return *this;
}

//
