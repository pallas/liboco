#ifndef CONTEXT_H
#define CONTEXT_H

#include "task.h"
#include <lace/do_not_copy.h>
#include <lite/queue.h>

#include <stdint.h>
#include <ucontext.h>

struct basic_context : public ucontext_t {
  lite::queue_link<basic_context> link;
};

class context : public basic_context, public lace::do_not_copy {
public:
  context();
  ~context();

  typedef lite::queue<basic_context, &basic_context::link> queue;
};

#endif//CONTEXT_H
