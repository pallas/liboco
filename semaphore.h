#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "trigger.h"
#include "context.h"
#include "do_not_copy.h"

#include <stdint.h>

class semaphore : public do_not_copy {
public:
  typedef uint64_t count_t;

  semaphore(count_t = 1);

  void wait();
  void signal();

private:
  trigger t;
  context::queue q;
};

#endif//SEMAPHORE_H
