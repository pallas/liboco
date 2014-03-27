#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "core.h"

#include "simple_task.h"
#include "socket_address.h"
#include "file_descriptor.h"

class simple_connector : public simple_task {
public:
  simple_connector(const socket_address &);
  void operator()();

  virtual void on_error();
  virtual void on_connect(int) = 0;

private:
  any_address a;
};

template <class R, class W>
class connector : public simple_connector {
public:
  connector(struct socket_address &addr)
    : simple_connector(addr)
  { }

  void on_connect(int fd) {
    file_descriptor::keepalive(fd);

    file_descriptor rfd(file_descriptor::dup(fd));
    file_descriptor wfd(file_descriptor::dup(fd));

    core::instance()
      .own(new R(static_cast<int>(rfd)))
      .own(new W(static_cast<int>(wfd)))
      ;

    rfd.release();
    wfd.release();
  }
};

template <class T>
class connector<T, void> : public simple_connector {
public:

  void on_connect(int fd) {
    file_descriptor::keepalive(fd);

    file_descriptor tfd(file_descriptor::dup(fd));

    core::instance().own(new T(tfd));

    tfd.release();
  }
};

#endif//CONNECTOR_H
