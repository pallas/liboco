#ifndef REACTOR_H
#define REACTOR_H

#include "context.h"
#include <lace/singleton.h>
#include "file_descriptor.h"

class trigger;

class reactor : public lace::singleton<reactor> {

  friend class lace::singleton<reactor>;
  reactor();

public:
  ~reactor();

  bool alone() const;
  bool armed() const;

  int poll(context::queue &);
  int wait(context::queue &);

private:
  const file_descriptor fd;

  unsigned armed_triggers;
  friend class trigger;
};

#endif//REACTOR_H
