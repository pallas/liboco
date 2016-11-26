#ifndef SIGNAL_KILLER_H
#define SIGNAL_KILLER_H

#include "signaler.h"
#include "killer.h"

class signal_killer : public signaler, public killer {
public:
  signal_killer(const signal_set &);
  void on_signal(const info_t &);
};

#endif//SIGNAL_KILLER_H
