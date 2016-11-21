#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "trigger.h"
#include "context.h"
#include <lace/do_not_copy.h>

#include <stdint.h>

class semaphore : public lace::do_not_copy {
public:
  typedef uint64_t count_t;

  semaphore(count_t = 1);

  class lock : public lace::do_not_copy {
  public:
    lock(semaphore &);
    ~lock();

  private:
    semaphore & s;
  };

  friend class lock;

private:
  void wait();
  void signal();

  trigger t;
  context::queue q;
};

#endif//SEMAPHORE_H
