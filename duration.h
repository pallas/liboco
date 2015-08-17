#ifndef DURATION_H
#define DURATION_H

#include <ctime>

class duration : public timespec {
public:
  duration(time_t s = 0, long n = 0);
  duration(double d);

  operator double () const;
};

#endif//DURATION_H
