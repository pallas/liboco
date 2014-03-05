#include "scheduler.h"

#include "try.h"
#include "context.h"
#include "demangler.h"

#include <cassert>

#include <iostream>
#include <typeinfo>
#include <stdexcept>

namespace {

  static class shiva_t : public ucontext_t {
  public:
    shiva_t() {
      TRY(getcontext, static_cast<ucontext_t*>(this));

      uc_stack.ss_sp = nirvana;
      uc_stack.ss_flags = 0;
      uc_stack.ss_size = sizeof nirvana;

      uc_link = NULL;
    }
    char nirvana[MINSIGSTKSZ]; // or SIGSTKSZ?
  } shiva;

  extern "C" void mukti(context * atman) { delete atman; }

  extern "C" void
  trampoline(unsigned * n, context::queue * q, context * c, task * t, bool d) {

    try {
      (*t)();
    } catch(const std::exception &e) {
      int status;
      std::cerr << demangler::instance().demangle(typeid(*t).name()) << ": "
                << demangler::instance().demangle(typeid(e).name()) << ": "
                << e.what() << std::endl;
    }

    if (d)
      delete t;
    --*n;

    assert(!q->empty());
    shiva.uc_link = q->dequeue();
    makecontext(static_cast<ucontext_t*>(&shiva), (void(*)())mukti, 1, c);
    TRY(setcontext, &shiva);

    assert(!"unreachable");

  }

}

scheduler::scheduler() : active_tasks(0), passive_tasks(0) { }
scheduler::~scheduler() { assert(done()); }

scheduler &
scheduler::add(task & t) {
  context * c = new context;
  makecontext(static_cast<ucontext_t*>(c), (void(*)())trampoline, 5, &++active_tasks, &todo, c, &t, false);
  todo.enqueue(c);
  return *this;
}

scheduler &
scheduler::eye(task & t) {
  context * c = new context;
  makecontext(static_cast<ucontext_t*>(c), (void(*)())trampoline, 5, &++passive_tasks, &todo, c, &t, false);
  todo.enqueue(c);
  return *this;
}

scheduler &
scheduler::own(task * t) {
  context * c = new context;
  makecontext(static_cast<ucontext_t*>(c), (void(*)())trampoline, 5, &++active_tasks, &todo, c, t, true);
  todo.enqueue(c);
  return *this;
}

void
scheduler::run() {
  while (!empty())
    yield();
}

bool
scheduler::empty() const {
  return todo.empty();
}

void
scheduler::yield() {
  assert(!todo.empty());

  ucontext_t current;
  todo.enqueue(&current);
  TRY(swapcontext, &current, todo.dequeue());
}

bool
scheduler::done() const {
  return 0 == active_tasks + passive_tasks;
}

bool
scheduler::active() const {
  return active_tasks > 0;
}

void
scheduler::defer(ucontext_t * c) {
  assert(!todo.empty());
  TRY(swapcontext, c, todo.dequeue());
}

void
scheduler::refer(ucontext_t * c) {
  todo.enqueue(c);
}

void
scheduler::transfer(context::queue & q) {
  assert(!q.empty());
  todo.chain(q);
}
//
