#include "reactor.h"

#include "try.h"
#include "trigger.h"

#include <cassert>
#include <unistd.h>
#include <sys/epoll.h>

reactor::reactor()
  : fd(file_descriptor(TRY(epoll_create1, EPOLL_CLOEXEC)))
  , total_triggers(0)
  , armed_triggers(0)
{ }

reactor::~reactor() {
  assert(alone());
  assert(!armed());
}

bool reactor::armed() const { return armed_triggers; }
bool reactor::alone() const { return !total_triggers; }

namespace {
  int
  pull_events(context::queue & q, int fd, int events_max, bool block) {
    struct epoll_event events[events_max];

    int n = epoll_wait(fd, events, events_max, block ? -1 : 0);
    if (n < 0)
      return 0;

    for (int i = 0 ; i < n ; ++i)
      if (events[i].data.ptr)
        q.enqueue(static_cast<trigger*>(events[i].data.ptr)->fire());

    return n;
  }
}

int
reactor::poll(context::queue & q) {
  const unsigned max = 16;

  int c = 0, n;

  do {
    n = pull_events(q, fd, max, false);
    c += n;
  } while (n == max);

  return c;
}

int
reactor::wait(context::queue & q) {
  assert(armed());
  const unsigned max = 16;

  int n;
  do {
    n = pull_events(q, fd, max, true);
  } while (!n);

  if (n == max)
    n += poll(q);

  return n;
}

unsigned
reactor::triggers() const {
  return total_triggers;
}

//
