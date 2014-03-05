#ifndef TASK_H
#define TASK_H

#include "do_not_copy.h"

class task : public do_not_copy {
public:
  virtual ~task();
  virtual void operator()() = 0;
};

#endif//TASK_H
