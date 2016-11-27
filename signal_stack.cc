#include "signal_stack.h"

#include <lace/try.h>
#include <lace/container_of.h>

#include <cassert>

namespace {
  bool stack_is_current(const stack_t & self) {
    stack_t test;
    TRY(::sigaltstack, NULL, &test);
    return true
        && test.ss_sp == self.ss_sp
        && test.ss_size == self.ss_size
        ;;
  }
}

signal_stack::signal_stack() : cookie(NULL)
{
  ss_sp = space;
  ss_size = sizeof space;
  ss_flags = 0;

  assert(MINSIGSTKSZ <= ss_size);

  TRY(::sigaltstack, static_cast<const stack_t*>(this), &save);
  assert(this == &current());
}

signal_stack::~signal_stack() {
  assert(stack_is_current(*this));
  TRY(::sigaltstack, &save, NULL);
  memset(static_cast<stack_t*>(this), 0, sizeof(stack_t));
}

signal_stack &
signal_stack::current() {
  stack_t test;
  TRY(::sigaltstack, NULL, &test);
  assert(test.ss_sp);

  typedef typeof(signal_stack::space) space_t;
  space_t & space = *reinterpret_cast<space_t*>(test.ss_sp);

  return lace::container_of<signal_stack, space_t>(space, &signal_stack::space);
}

void * signal_stack::base() const { return ss_sp; }
std::size_t signal_stack::size() const { return ss_size; }

//
