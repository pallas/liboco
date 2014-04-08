#ifndef CONTEXT_H
#define CONTEXT_H

#include "task.h"
#include "do_not_copy.h"
#include "intrusive_queue.h"

#include <stdint.h>
#include <ucontext.h>

struct basic_context : public ucontext_t {
  intrusive_queue_link<basic_context> link;
};

class context : public basic_context, public do_not_copy {
public:
  context();
  ~context();

  typedef intrusive_queue<basic_context, &basic_context::link> queue;
};

#endif//CONTEXT_H
