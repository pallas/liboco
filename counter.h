#ifndef COUNTER_H
#define COUNTER_H

#include "simple_task.h"

#include <stdint.h>

class counter : public simple_task {
public:
  typedef uint64_t count_t;

  counter(count_t = 0);
  void operator()();

  void increment(count_t = 1);

  virtual void on_count(count_t) = 0;
};

#endif//COUNTER_H
