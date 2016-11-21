#include "inet_address.h"

#include <lace/try.h>

#include <cassert>
#include <cstring>
#include <arpa/inet.h>

inet_address::inet_address(in_port_t port) {
  sa.sin_port = htons(port);
  sa.sin_addr.s_addr = INADDR_ANY;
}

inet_address::inet_address(in_port_t port, const in_addr_t & addr) {
  sa.sin_port = htons(port);
  sa.sin_addr.s_addr = addr;
}

inet_address::inet_address(in_port_t port, const char * addr) {
  sa.sin_port = htons(port);
  if (!TRY(inet_pton, family(), addr, &sa.sin_addr))
    sa.sin_addr.s_addr = INADDR_NONE;
}

inet_address::inet_address(const socket_address & o) {
  assert(af == o.family());
  assert(o.length() <= sizeof sa);
  memcpy(&sa, &o.cast(), sizeof sa);
}

inet_address &
inet_address::operator=(const socket_address & o) {
  assert(af == o.family());
  assert(o.length() <= sizeof sa);
  memcpy(&sa, &o.cast(), sizeof sa);
  return *this;
}

//
