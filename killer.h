#ifndef KILLER_H
#define KILLER_H

#include <lite/stack.h>

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
    lite::stack_link<mark>::type link;
  };

  lite::stack<mark, &mark::link> hitlist;
};

#endif//KILLER_H
