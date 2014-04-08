#ifndef INTRUSIVE_ORDER_H
#define INTRUSIVE_ORDER_H

#include "do_not_copy.h"
#include "intrusive_link.h"

#include <cassert>
#include <cstddef>
#include <algorithm>

template <class T>
struct intrusive_order_link : public intrusive_link<T> {
  typedef intrusive_order_link type;
};

template <class T, typename intrusive_order_link<T>::type T::*link,
          bool (T::*predicate)(const T &) const>
class intrusive_order : public do_not_copy {
public:
  intrusive_order() : head(NULL), tail(&head) { }
  ~intrusive_order() { assert(empty()); }

  bool empty() const { return !head; }

  intrusive_order & insert(T* t) {
    assert(NULL == (t->*link).p);
    if (empty())
      assert(&head == tail);
    if (empty() || !(t->*predicate)(back())) {
      *tail = t;
      tail = &(t->*link).p;
      *tail = t;
    } else {
      T ** c;
      for (c = &head ; c != tail ; c = &((*c)->*link).p)
        if ((t->*predicate)(**c))
          break;
      (t->*link).p = *c;
      *c = t;
    }
    assert(!empty());
    return *this;
  }

  T & front() {
    assert(!empty());
    return *head;
  }

  const T & front() const {
    assert(!empty());
    return *head;
  }

  T & back() {
    assert (!empty());
    return **tail;
  }

  const T & back() const {
    assert (!empty());
    return **tail;
  }

  T* remove() {
    assert(!empty());
    T* t = head;

    head = *tail != head
         ? (head->*link).p
         : NULL;

    if (empty())
      tail = &head;

    (t->*link).p = NULL;
    return t;
  }

  intrusive_order & merge(intrusive_order & that) {
    assert(this != &that);
    intrusive_order result;

    while (!this->empty() && !that.empty())
      result.insert( (this->front().*predicate)(that.front())
                   ? this->remove()
                   : that.remove() );

    if (!this->empty())
      result.chain(*this);

    if (!that.empty())
      result.chain(that);

    swap(result);
    return *this;
  }

  void swap(intrusive_order & that) {
    std::swap(this->head, that.head);
    std::swap(this->tail, that.tail);
  }

private:
  T * head;
  T ** tail;

  intrusive_order & chain(intrusive_order & that) {
    assert(this != &that);
    assert(!that.empty());

    *tail = that.head;
    tail = that.tail;
    assert(!empty());

    that.head = NULL;
    that.tail = &that.head;
    assert(that.empty());

    return *this;
  }
};

#endif//INTRUSIVE_ORDER
