#include "local_address.h"

#include "try.h"

#include <cassert>
#include <cstring>

local_address::local_address() { }

local_address::local_address(const char * name) {
  strncpy(sa.sun_path, name, sizeof sa.sun_path);
}

local_address::local_address(const socket_address & o) {
  assert(af == o.family());
  assert(o.length() <= sizeof sa);
  memcpy(&sa, &o.cast(), sizeof sa);
}

local_address &
local_address::operator=(const socket_address & o) {
  assert(af == o.family());
  assert(o.length() <= sizeof sa);
  memcpy(&sa, &o.cast(), sizeof sa);
  return *this;
}

//
