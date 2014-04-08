#ifndef TRIGGER_H
#define TRIGGER_H

#include "do_not_copy.h"
#include "file_descriptor.h"
#include "context.h"

#include <sys/epoll.h>

class trigger : public basic_context {
public:
  explicit trigger(int); // takes ownership
  trigger(const file_descriptor &); // dups
  ~trigger();

  int fd() const;
  int dup() const;

  basic_context * wait_for_read();
  basic_context * wait_for_write();
  basic_context * wait_for_error();

  basic_context * wait_for_nothing();
  basic_context * wait_for_anything();

  bool readable() const;
  bool writable() const;

  basic_context * fire();

  bool armed() const;

private:
  void arm();
  void disarm();

  file_descriptor fd_;
  struct epoll_event ev_;
};

#endif//TRIGGER_H
