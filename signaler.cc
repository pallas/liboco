#include "signaler.h"

#include "try.h"

#include "core.h"
#include "signal_set.h"

#include <cassert>
#include <unistd.h>

#include <sys/signalfd.h>

namespace {

  static const int flags = SFD_NONBLOCK | SFD_CLOEXEC;

}

signaler::signaler(const signal_set & ss)
  : simple_task(TRY(signalfd, -1, &ss, flags))
{ }

void
signaler::operator()() {
  info_t si;
  while (!dead()) {
    int r = TRY_ERR(EAGAIN, read, t.fd(), &si, sizeof si);
    if (r < 0) {
      core::wait_for_read(t);
      continue;
    }
    assert(r == sizeof si);
    on_signal(si);
  }
}

//
