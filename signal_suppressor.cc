#include "signal_suppressor.h"

#include "try.h"

#include <cassert>
#include <cstddef>
#include <errno.h>

signal_suppressor::signal_suppressor(int s)
  : signal(s)
  , mask(signal)
{ suppress(); }

signal_suppressor::~signal_suppressor() { restore(); }

void
signal_suppressor::suppress() {
  if ((was_pending = signal_set().pending().has(signal)))
    return;

  if (!signal_set().current().has(signal))
    blocked.add(signal).block();
}

void
signal_suppressor::restore() {
  if (was_pending)
    return;

  if (signal_set().pending().has(signal)) {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 0;

    while (TRY_ERR(EINTR, sigtimedwait, &mask, NULL, &ts) < 0) { }

    assert(!signal_set().pending().has(signal));
  }

  if (blocked.has(signal)) {
    blocked.unblock();
    blocked.del(signal);
  }
}

//
