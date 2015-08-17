#ifndef TIMER_H
#define TIMER_H

#include "duration.h"
#include "simple_task.h"

class timer : public simple_task {
public:
  timer(duration, bool realtime = false);
  void operator()();

  virtual void on_tick() = 0;
};

#endif//TIMER_H
