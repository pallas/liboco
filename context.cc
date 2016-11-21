#include "context.h"

#include "stack.h"
#include <lace/try.h>

#include <cassert>

context::context() {
  ucontext_t* self = static_cast<ucontext_t*>(this);
  TRY(getcontext, self);
  stack::init(&self->uc_stack);
}

context::~context() {
  ucontext_t* self = static_cast<ucontext_t*>(this);
  stack::fini(&self->uc_stack);
}

//
