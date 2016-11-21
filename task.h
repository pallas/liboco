#ifndef TASK_H
#define TASK_H

#include <lace/do_not_copy.h>

class task : public lace::do_not_copy {
public:
  virtual ~task();
  virtual void operator()() = 0;
};

#endif//TASK_H
