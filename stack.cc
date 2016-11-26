#include "stack.h"
#include "resource_limit.h"

#include <cassert>
#include <cstring>
#include <cstddef>
#include <stdint.h>
#include <unistd.h>
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
#ifdef MAP_NORESERVE
    | MAP_NORESERVE
#endif
    ;

    static long page_size = sysconf(_SC_PAGE_SIZE);
    static long page_mask = page_size - 1;

    void* adjust(void* p, ptrdiff_t d) {
      return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(p) + d);
    }
};

stack::stack() throw (std::bad_alloc)
{
  ss_flags = 0;

  ss_size = stack_size;
  assert(MINSIGSTKSZ <= ss_size);

  assert(0 == (page_size & (page_size-1)));

  ss_size += 2 * page_size;

  const long page_mask = page_size - 1;
  ss_size += page_mask;
  ss_size &= ~page_mask;

  ss_sp = mmap(NULL, ss_size, stack_prot, stack_flags, -1, 0);
  if (MAP_FAILED == ss_sp)
    throw std::bad_alloc();

  ss_sp = adjust(ss_sp, page_size);
  ss_size -= 2 * page_size;

  TRY(mprotect, adjust(ss_sp, -page_size), page_size, PROT_NONE);
  TRY(mprotect, adjust(ss_sp, ss_size), page_size, PROT_NONE);
}

stack::~stack() {
  TRY(munmap, adjust(ss_sp, -page_size), ss_size + 2 * page_size);
  memset(this, 0, sizeof this);
}

void * stack::base() { return ss_sp; }
std::size_t stack::size() const { return ss_size; }

//
