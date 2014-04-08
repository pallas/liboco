#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"
#include "context.h"
#include "singleton.h"
#include "intrusive_queue.h"

#include <ucontext.h>

class scheduler : public singleton<scheduler> {

  friend class singleton<scheduler>;
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
