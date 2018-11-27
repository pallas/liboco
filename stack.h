#ifndef STACK_H
#define STACK_H

#include <new>
#include <signal.h>

#include <lace/do_not_copy.h>

class stack : public stack_t, public lace::do_not_copy {
public:
  stack();
  ~stack();

  void * base() const;
  std::size_t size() const;

  template <class T>
  static stack* cast(T* tp) {
    return reinterpret_cast<stack*>(tp);
  }

  template <class T>
  static stack* init(T* tp) {
    return new (cast(tp)) stack;
  }

  template <class T>
  static void fini(T* tp) {
    cast(tp)->~stack();
  }
};

#endif//STACK_H
