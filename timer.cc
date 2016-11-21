#include "timer.h"

#include <lace/try.h>

#include "core.h"

#include <cassert>
#include <unistd.h>

#include <sys/timerfd.h>

namespace {

  static const int flags = TFD_NONBLOCK | TFD_CLOEXEC;

}

timer::timer(duration d, bool realtime)
  : simple_task(TRY(timerfd_create, realtime ? CLOCK_REALTIME : CLOCK_MONOTONIC, flags))
{
  struct itimerspec its = { .it_interval = d, .it_value = d };
  TRY(timerfd_settime, t.fd(), 0, &its, NULL);
}

void
timer::operator()() {
  uint64_t c;
  while (!dead()) {
    int r = TRY_ERR(EAGAIN, read, t.fd(), &c, sizeof c);
    if (r < 0) {
      core::wait_for_read(t);
      continue;
    }
    assert(r == sizeof c);
    assert(c);

    while (!dead() && c-->0)
      on_tick();
  }
}

//
