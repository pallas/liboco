#include "killer.h"

#include "killable.h"

#include <cassert>

killer::~killer() {
  while (!hitlist.empty())
    delete hitlist.pop();
}

killer &
killer::target(killable & k) {
  hitlist.push(new mark(k));
  return *this;
}

bool
killer::empty() const {
  return hitlist.empty();
}

void
killer::killone() {
  assert(!empty());
  mark * m = hitlist.pop();
  m->k.kill();
  delete m;
}

void
killer::killall() {
  while (!empty())
    killone();
}

killer::mark::mark(killable & kk)
  : k(kk)
  , m(NULL)
{ }

//
