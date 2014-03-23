#include "killable.h"

#include <cassert>

killable::killable() : dead_(false) { }

void
killable::kill() {
  if (!dead())
    die();
  dead_ = true;
  assert(dead());
}

bool killable::dead() const { return dead_; }

void killable::die() { assert(!dead()); }

//
