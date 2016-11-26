#ifndef SIGNAL_STACK_H
#define SIGNAL_STACK_H

#include <new>
#include <signal.h>

#include <lace/do_not_copy.h>

class signal_stack : public stack_t, public lace::do_not_copy {
public:
  signal_stack();
  ~signal_stack();

  void * base() const;
  std::size_t size() const;

private:
  static signal_stack & current();

  stack_t save;
  unsigned char space[SIGSTKSZ];
};

#endif//SIGNAL_STACK_H
