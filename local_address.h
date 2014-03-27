#ifndef LOCAL_ADDRESS_H
#define LOCAL_ADDRESS_H

#include "socket_address.h"

#include <sys/un.h>

class local_address : public basic_socket_address<AF_LOCAL,sockaddr_un> {
public:
  local_address();
  local_address(const char *);

  local_address(const socket_address &);
  local_address & operator=(const socket_address &);
};

#endif//LOCAL_ADDRESS_H
