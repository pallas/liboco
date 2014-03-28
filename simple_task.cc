#include "simple_task.h"

#include "core.h"

simple_task::simple_task(int fd)
  : t(fd)
{ }

void
simple_task::die() {
  if (t.armed())
    core::wait_for_nothing(t);
}

//
