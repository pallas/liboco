#ifndef SIGNALER_H
#define SIGNALER_H

#include "simple_task.h"

class signal_set;
struct signalfd_siginfo;

class signaler : public simple_task {
public:
  typedef struct signalfd_siginfo info_t;

  signaler(const signal_set &);
  void operator()();

  virtual void on_signal(const info_t &) = 0;
};

#endif//SIGNALER_H
