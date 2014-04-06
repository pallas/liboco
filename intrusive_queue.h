#ifndef INTRUSIVE_QUEUE_H
#define INTRUSIVE_QUEUE_H

#include "do_not_copy.h"

#include <cassert>
#include <cstddef>

template <class T, T* T::*link>
class intrusive_queue : public do_not_copy {
public:
  intrusive_queue() : head(NULL), tail(&head) { }
  ~intrusive_queue() { assert(empty()); }

  bool empty() const { return !head; }

  intrusive_queue & enqueue(T* t) {
    assert(NULL == t->*link);
    if (empty())
      assert(&head == tail);
    *tail = t;
    tail = &(t->*link);
    *tail = NULL;
    assert(!empty());
    return *this;
  }

  T* peek() const {
    assert(!empty());
    return head;
  }

  T* dequeue() {
    assert(!empty());
    T* t = head;

    if (!(head = head->*link))
      tail = &head;

    t->*link = NULL;
    return t;
  }

  intrusive_queue & chain(intrusive_queue & that) {
    assert(!that.empty());
    *tail = that.head;
    tail = that.tail;

    that.head = NULL;
    that.tail = &that.head;

    assert(that.empty());
    return *this;
  }

private:
  T * head;
  T ** tail;
};

#endif//INTRUSIVE_QUEUE
