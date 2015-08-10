#include "semaphore.h"

#include "try.h"

#include "core.h"

#include <cassert>
#include <unistd.h>

#include <sys/eventfd.h>

namespace {

  static const int flags = EFD_NONBLOCK | EFD_CLOEXEC | EFD_SEMAPHORE;

}

semaphore::semaphore(count_t c)
  : t(TRY(eventfd, c, flags))
{ }

void
semaphore::wait() {
  count_t c = 0;
  int r = TRY_ERR(EAGAIN, read, t.fd(), &c, sizeof c);
  if (r < 0) {
    trigger x(t.dup());
    do {
      core::wait_for_read(x);
      r = TRY_ERR(EAGAIN, read, x.fd(), &c, sizeof c);
    } while (r < 0);
  }
  assert(r == sizeof c);
  assert(c == 1);
}
void
semaphore::signal() {
  count_t c = 1;
  int w = TRY_ERR(EAGAIN, write, t.fd(), &c, sizeof c);
  if (w < 0) {
    trigger x(t.dup());
    do {
      core::wait_for_write(x);
      w = TRY_ERR(EAGAIN, write, x.fd(), &c, sizeof c);
    } while (w < 0);
  }
  assert(w == sizeof c);
}

//
