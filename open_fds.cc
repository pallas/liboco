#include <fcntl.h>
#include <cstdlib>
#include <iostream>

#include <sys/resource.h>

#include "try.h"

int main(int argc, char *argv[]) {
  struct rlimit rl;
  TRY(getrlimit, RLIMIT_NOFILE, &rl);
  unsigned c = 0;
  for (int i = 0 ; i < rl.rlim_cur ; ++i)
    if (0 <= TRY_ERR(EBADF, fcntl, i, F_GETFL))
      std::cout << *argv << '\t' << c++ << '\t' << i << std::endl;

  return EXIT_SUCCESS;
}

//
