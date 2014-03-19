#include "signal_set.h"

#include "try.h"

#include <cassert>

signal_set::signal_set() {
  TRY(sigemptyset, static_cast<sigset_t*>(this));
}

signal_set::signal_set(int signal) {
  TRY(sigemptyset, static_cast<sigset_t*>(this));
  TRY(sigaddset, static_cast<sigset_t*>(this), signal);
}

signal_set &
signal_set::add(int signal) {
  assert(!has(signal));
  TRY(sigaddset, static_cast<sigset_t*>(this), signal);
  return *this;
}

signal_set &
signal_set::del(int signal) {
  assert(has(signal));
  TRY(sigdelset, static_cast<sigset_t*>(this), signal);
  return *this;
}

bool
signal_set::has(int signal) const {
  return TRY(sigismember, static_cast<const sigset_t*>(this), signal);
}

bool
signal_set::empty() const {
  for (int i = 1 ; i < NSIG ; ++i)
    if (has(i))
      return false;

  return true;
}

const signal_set &
signal_set::block() const {
  TRY(sigprocmask, SIG_BLOCK, static_cast<const sigset_t*>(this), NULL);
  return *this;
}

const signal_set &
signal_set::unblock() const {
  TRY(sigprocmask, SIG_UNBLOCK, static_cast<const sigset_t*>(this), NULL);
  return *this;
}

signal_set &
signal_set::install() {
  TRY(sigprocmask, SIG_SETMASK, static_cast<const sigset_t*>(this), NULL);
  return *this;
}

signal_set &
signal_set::current() {
  TRY(sigprocmask, SIG_SETMASK, NULL, static_cast<sigset_t*>(this));
  return *this;
}

signal_set &
signal_set::pending() {
  TRY(sigpending, static_cast<sigset_t*>(this));
  return *this;
}

//
