#ifndef SIGNAL_SET_H
#define SIGNAL_SET_H

#include <signal.h>

class signal_set : public sigset_t {
public:
  signal_set();
  signal_set(int);

  signal_set & add(int);
  signal_set & del(int);

  bool has(int) const;
  bool empty() const;

  const signal_set & block() const;
  const signal_set & unblock() const;

  signal_set & install();
  signal_set & current();
  signal_set & pending();
};

#endif//SIGNAL_SET_H
