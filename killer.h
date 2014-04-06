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
    ~mark();

    killable & k;
    struct mark * m;
  };

  intrusive_stack<mark, &mark::m> hitlist;
};

#endif//KILLER_H
