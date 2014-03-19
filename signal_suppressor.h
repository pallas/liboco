#ifndef SIGNAL_SUPPRESSOR
#define SIGNAL_SUPPRESSOR

#include "signal_set.h"

class signal_suppressor {
public:
  signal_suppressor(int);
  ~signal_suppressor();

  void suppress();
  void restore();

private:
  bool was_pending;
  const int signal;
  const signal_set mask;
  signal_set blocked;
};

#endif//SIGNAL_SUPPRESSOR
