#ifndef DEMANGLER_H
#define DEMANGLER_H

#include "singleton.h"

#include <string>
#include <cstddef>

class demangler : public singleton<demangler> {

  friend class singleton<demangler>;
  demangler();

public:
  ~demangler();

  std::string demangle(const char *);

private:
  char * buf;
  size_t len;
};

#endif//DEMANGLER_H
