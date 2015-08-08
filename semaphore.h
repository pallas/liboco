#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "trigger.h"

#include <stdint.h>

class semaphore {
public:
  typedef uint64_t count_t;

  semaphore(count_t = 1);

  void wait();
  void signal();

private:
  trigger t;
};

#endif//SEMAPHORE_H
