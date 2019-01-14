#include "core.h"

#include "semaphore.h"

#include <cassert>
#include <cstdlib>
#include <unistd.h>

#include <iostream>

#include <lace/random.h>
#include <lace/singleton.h>

lace::random & rng = lace::singleton<lace::random>::instance();

class exclusive_task : public task {
public:

  void operator() () {
    if (rng.l() >= 0)
      core::instance().schedule();

    semaphore::lock l(s);
    const unsigned n = 8;
    for (unsigned i = 0 ; i < n ; ++i) {
      std::cout << this << " #" << i << std::endl;
      if (rng.l() >= 0)
        core::instance().schedule();
    }
  }

private:
    static semaphore s;
};

semaphore exclusive_task::s;

int main(int argc, char *argv[]) {
  core & c = core::instance();

  c.own(new exclusive_task)
   .own(new exclusive_task)
   .own(new exclusive_task)
   .own(new exclusive_task)
   .react();

  return EXIT_SUCCESS;
}

//
