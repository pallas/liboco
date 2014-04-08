#ifndef KILLER_H
#define KILLER_H

#include "intrusive_stack.h"

class killable;

class killer {
public:
  ~killer();

  killer & target(killable &);

  bool empty() const;

  void killone();
  void killall();

private:
  struct mark {
    mark(killable &);

    killable & k;
    intrusive_stack_link<mark>::type link;
  };

  intrusive_stack<mark, &mark::link> hitlist;
};

#endif//KILLER_H
