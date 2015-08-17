#include "core.h"

#include "timer.h"

#include <cassert>
#include <cstdlib>

#include <iostream>

class timed_task : public timer {
public:

  timed_task(int times) : timer(1.0/double(times)), c(0), n(times) { }
  void on_tick() {
    std::cout << "tick on #" << n << std::endl;
    if (++c == n)
      kill();
  }

  ~timed_task() { assert(c == n); }

private:
  unsigned c;
  unsigned n;
};

int main(int argc, char *argv[]) {
  core & c = core::instance();

  c.own(new timed_task(2))
   .own(new timed_task(3))
   .own(new timed_task(5))
   .own(new timed_task(7))
   .own(new timed_task(11))
   .react();

  return EXIT_SUCCESS;
}

//
