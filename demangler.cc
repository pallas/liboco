#include "demangler.h"

#include <cstdlib>
#include <cxxabi.h>

demangler::demangler() : buf(NULL), len(0) { }
demangler::~demangler() { free(buf); }

std::string
demangler::demangle(const char * name) {
  int status;
  size_t new_len = len;
  char * new_buf = abi::__cxa_demangle(name, buf, &new_len, &status);
  if (!new_buf)
    return std::string(name);
  buf = new_buf;
  if (len < new_len)
    len = new_len;
  return std::string(new_buf);
}

//
