#include "counter.h"

#include "try.h"

#include "core.h"

#include <cassert>
#include <unistd.h>

#include <sys/eventfd.h>

namespace {

  static const int flags = EFD_NONBLOCK | EFD_CLOEXEC;

}

counter::counter(count_t c)
  : simple_task(TRY(eventfd, c, flags))
{ }

void
counter::operator()() {
  count_t c;
  while (!dead()) {
    int r = TRY_ERR(EAGAIN, read, t.fd(), &c, sizeof c);
    if (r < 0) {
      core::wait_for_read(t);
      continue;
    }
    assert(r == sizeof c);
    on_count(c);
  }
}

void
counter::increment(count_t c) {
  while (true) {
    int w = TRY_ERR(EAGAIN, write, t.fd(), &c, sizeof c);
    if (w < 0) {
      core::wait_for_write(t);
      continue;
    }
    assert(w == sizeof c);
    return;
  }
}

//
