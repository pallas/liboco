#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"
#include "context.h"
#include <lace/singleton.h>

#include <ucontext.h>

class scheduler : public lace::singleton<scheduler> {

  friend class lace::singleton<scheduler>;
  scheduler();

public:
  ~scheduler();

  scheduler & add(task &);
  scheduler & eye(task &);
  scheduler & own(task *);
  void run();

  bool empty() const;
  void yield();

  bool done() const;
  bool active() const;

  void defer(basic_context *);
  void refer(basic_context *);
  void transfer(context::queue &);

private:
  context::queue todo;
  unsigned active_tasks, passive_tasks;
};

#endif//SCHEDULER_H
