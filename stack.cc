#include "stack.h"
#include "resource_limit.h"

#include <cassert>
#include <cstring>
#include <sys/mman.h>

namespace {
  static std::size_t stack_size = resource_limit::cur<RLIMIT_STACK>();

  static int stack_prot = 0
    | PROT_READ
    | PROT_WRITE
    ;

  static int stack_flags = 0
    | MAP_PRIVATE
    | MAP_ANONYMOUS
    | MAP_STACK
#ifdef MAP_UNINITIALIZED
    | MAP_UNINITIALIZED
#endif
    ;
};

stack::stack() throw (std::bad_alloc)
{
  ss_flags = 0;

  ss_size = stack_size;
  assert(MINSIGSTKSZ <= ss_size);

  ss_sp = mmap(NULL, ss_size, stack_prot, stack_flags, -1, 0);
  if (MAP_FAILED == ss_sp)
    throw std::bad_alloc();
}

stack::~stack() {
  TRY(munmap, ss_sp, ss_size);
  memset(this, 0, sizeof this);
}

void * stack::base() { return ss_sp; }
std::size_t stack::size() const { return ss_size; }

//
