#ifndef SOCKET_ADDRESS_H
#define SOCKET_ADDRESS_H

#include <cstring>
#include <sys/socket.h>

class socket_address {
public:

  virtual sockaddr & cast() = 0;
  virtual const sockaddr & cast() const = 0;
  virtual socklen_t length() const = 0;

  int protocol() const;

  int family() const;

  bool is_inet() const;
  bool is_inet6() const;
  bool is_local() const;
};

template <int AF, typename SA>
class basic_socket_address : public socket_address {
public:
  basic_socket_address() {
    memset(&sa, 0, sizeof sa);
    cast().sa_family = AF;
  }

  sockaddr & cast() {
    return reinterpret_cast<sockaddr &>(sa);
  }

  const sockaddr & cast() const {
    return reinterpret_cast<const sockaddr &>(sa);
  }

  socklen_t length() const { return sizeof sa; }

protected:
  SA sa;
  static const int af = AF;
};

class any_address : public basic_socket_address<AF_UNSPEC,sockaddr_storage> {
public:
  any_address();

  any_address(const socket_address &);
  any_address & operator=(const socket_address &);
};

#endif//SOCKET_ADDRESS_H
