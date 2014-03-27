#include "inet6_address.h"

#include "try.h"

#include <cassert>
#include <cstring>
#include <arpa/inet.h>

inet6_address::inet6_address(in_port_t port) {
  sa.sin6_port = htons(port);
  sa.sin6_addr = in6addr_any;
}

inet6_address::inet6_address(in_port_t port, const struct in6_addr & addr) {
  sa.sin6_port = htons(port);
  sa.sin6_addr = addr;
}

inet6_address::inet6_address(in_port_t port, const char * addr) {
  sa.sin6_port = htons(port);
  if (!TRY(inet_pton, family(), addr, &sa.sin6_addr))
    memset(&sa.sin6_addr, -1, sizeof sa.sin6_addr);
}

inet6_address::inet6_address(const socket_address & o) {
  assert(af == o.family());
  assert(o.length() <= sizeof sa);
  memcpy(&sa, &o.cast(), sizeof sa);
}

inet6_address &
inet6_address::operator=(const socket_address & o) {
  assert(af == o.family());
  assert(o.length() <= sizeof sa);
  memcpy(&sa, &o.cast(), sizeof sa);
  return *this;
}

//
