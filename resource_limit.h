#ifndef RESOURCE_LIMIT_H
#define RESOURCE_LIMIT_H

#include <lace/try.h>
#include <cassert>
#include <sys/resource.h>

namespace resource_limit {
  template <int resource>
  struct rlimit get() {
    struct rlimit rl;
    TRY(getrlimit, resource, &rl);
    return rl;
  };

  template <int resource>
  void set(rlim_t limit) {
    struct rlimit rl = get<resource>();
    rl.rlim_cur = limit;
    assert(rl.rlim_cur <= rl.rlim_max);
    TRY(setrlimit, resource, &rl);
  };

  template <int resource>
  void hog() {
    struct rlimit rl = get<resource>();
    rl.rlim_cur = rl.rlim_max;
    TRY(setrlimit, resource, &rl);
  };

  template <int resource> rlim_t cur() { return get<resource>().rlim_cur; }
  template <int resource> rlim_t max() { return get<resource>().rlim_max; }
};

#endif//RLIMIT_H
