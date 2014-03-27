#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "core.h"

#include "simple_task.h"
#include "socket_address.h"
#include "file_descriptor.h"

class simple_acceptor : public simple_task {
public:
  simple_acceptor(const socket_address &);
  void operator()();

  virtual void on_error();
  virtual void on_accept(int, const socket_address &) = 0;
};

template <class T>
class acceptor : public simple_acceptor {
public:
  acceptor(const socket_address &addr)
    : simple_acceptor(addr)
  { }

  void on_accept(int fd, const socket_address &addr) {
    file_descriptor::keepalive(fd);

    file_descriptor tfd(fd);

    core::instance().own(new T(static_cast<int>(tfd)));

    tfd.release();
  }
};

#endif//ACCEPTOR_H
