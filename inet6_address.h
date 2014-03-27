#ifndef INET6_ADDRESS_H
#define INET6_ADDRESS_H

#include "socket_address.h"

#include <netinet/in.h>

class inet6_address : public basic_socket_address<AF_INET6,sockaddr_in6> {
public:
  inet6_address(in_port_t);
  inet6_address(in_port_t, const struct in6_addr & addr);
  inet6_address(in_port_t port, const char * addr);

  inet6_address(const socket_address &);
  inet6_address & operator=(const socket_address &);
};

#endif//INET6_ADDRESS_H
