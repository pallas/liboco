#ifndef TRIGGER_H
#define TRIGGER_H

#include "do_not_copy.h"
#include "file_descriptor.h"

#include <ucontext.h>
#include <sys/epoll.h>

class trigger : public ucontext_t {
public:
  explicit trigger(int); // takes ownership
  trigger(const file_descriptor &); // dups
  ~trigger();

  int fd() const;
  int dup() const;

  ucontext_t * wait_for_read();
  ucontext_t * wait_for_write();
  ucontext_t * wait_for_error();

  ucontext_t * wait_for_nothing();
  ucontext_t * wait_for_anything();

  bool readable() const;
  bool writable() const;

  ucontext_t * fire();

  bool armed() const;

private:
  void arm();
  void disarm();

  file_descriptor fd_;
  struct epoll_event ev_;
};

#endif//TRIGGER_H
