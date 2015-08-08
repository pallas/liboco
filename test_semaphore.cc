#include "core.h"

#include "killer.h"
#include "semaphore.h"

#include <cassert>
#include <cstdlib>
#include <unistd.h>

#include <iostream>

class exclusive_task : public task {
public:

  void operator() () {
    if (rand() > (RAND_MAX/2))
      core::instance().schedule();

    s.wait();
    const unsigned n = 8;
    for (unsigned i = 0 ; i < n ; ++i) {
      std::cout << this << " #" << i << std::endl;
      if (rand() > (RAND_MAX/2))
        core::instance().schedule();
    }
    s.signal();
  }

private:
    static semaphore s;
};

semaphore exclusive_task::s;

int main(int argc, char *argv[]) {
  srand(getpid());

  core & c = core::instance();

  c.own(new exclusive_task)
   .own(new exclusive_task)
   .own(new exclusive_task)
   .own(new exclusive_task)
   .react();

  return EXIT_SUCCESS;
}

//
