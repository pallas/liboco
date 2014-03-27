#ifndef INET_ADDRESS_H
#define INET_ADDRESS_H

#include "socket_address.h"

#include <netinet/in.h>

class inet_address : public basic_socket_address<AF_INET,sockaddr_in> {
public:
  inet_address(in_port_t);
  inet_address(in_port_t, const in_addr_t & addr);
  inet_address(in_port_t port, const char * addr);

  inet_address(const socket_address &);
  inet_address & operator=(const socket_address &);
};

#endif//INET_ADDRESS_H
