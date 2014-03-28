#ifndef SIMPLE_TASK_H
#define SIMPLE_TASK_H

#include "task.h"
#include "trigger.h"
#include "killable.h"

class simple_task : public task, public killable {
public:
  simple_task(int);
  void die();

protected:
  trigger t;
};

#endif//SIMPLE_TASK_H
