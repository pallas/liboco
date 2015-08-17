#include "duration.h"

namespace {

  static const int nsec_in_sec = 1e9;

}

duration::duration(time_t s, long n) {
  tv_sec = s;
  tv_nsec = n;
}

duration::duration(double d) {
  tv_sec = static_cast<typeof(tv_sec)>(d);
  tv_nsec = (d-tv_sec) * nsec_in_sec;
}

duration::operator double () const {
  return tv_sec + double(tv_nsec)/double(nsec_in_sec);
}

//
