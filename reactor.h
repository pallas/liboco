#ifndef REACTOR_H
#define REACTOR_H

#include "context.h"
#include "do_not_copy.h"
#include "singleton.h"
#include "file_descriptor.h"

class trigger;

class reactor : public singleton<reactor> {

  friend class singleton<reactor>;
  reactor();

public:
  ~reactor();

  bool alone() const;
  bool armed() const;

  int poll(context::queue &);
  int wait(context::queue &);

  unsigned triggers() const;

private:
  const file_descriptor fd;

  unsigned total_triggers;
  unsigned armed_triggers;
  friend class trigger;
};

#endif//REACTOR_H
