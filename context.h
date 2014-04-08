#ifndef CONTEXT_H
#define CONTEXT_H

#include "task.h"
#include "do_not_copy.h"
#include "intrusive_queue.h"

#include <stdint.h>
#include <ucontext.h>

struct basic_context : public ucontext_t {
  basic_context() { uc_link = NULL; }
};

class context : public basic_context, public do_not_copy {
public:
  context();
  ~context();

  typedef intrusive_queue<ucontext_t, &ucontext_t::uc_link> queue;
};

#endif//CONTEXT_H
