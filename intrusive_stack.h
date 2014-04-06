#ifndef INTRUSIVE_STACK_H
#define INTRUSIVE_STACK_H

#include "do_not_copy.h"

#include <cassert>
#include <cstddef>

template <class T, T* T::*link>
class intrusive_stack : public do_not_copy {
public:
  intrusive_stack() : head(NULL) { }
  ~intrusive_stack() { assert(empty()); }

  bool empty() const { return !head; }

  intrusive_stack & push(T* t) {
    assert(NULL == t->*link);
    t->*link = head;
    head = t;
    return *this;
  }

  T* peek() const {
    assert(!empty());
    return head;
  }
  
  T* pop() {
    assert(!empty());
    T* t = head;
    head = t->*link;
    t->*link = NULL;
    return t;
  }

private:
  T * head;
};

#endif//INTRUSIVE_STACK_H
