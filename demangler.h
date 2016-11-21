#ifndef DEMANGLER_H
#define DEMANGLER_H

#include <lace/singleton.h>

#include <string>
#include <cstddef>

class demangler : public lace::singleton<demangler> {

  friend class lace::singleton<demangler>;
  demangler();

public:
  ~demangler();

  std::string demangle(const char *);

private:
  char * buf;
  size_t len;
};

#endif//DEMANGLER_H
