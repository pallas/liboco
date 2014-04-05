#include "core.h"

#include "try.h"

#include "context.h"
#include "reactor.h"
#include "trigger.h"
#include "scheduler.h"
#include "signal_set.h"
#include "file_descriptor.h"
#include "signal_suppressor.h"

#include <pwd.h>
#include <grp.h>

#include <cassert>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>

namespace {

  class queue : public singleton<queue>, public context::queue { };

}

core::core()
  : events_(0)
  , read_waits_(0)
  , write_waits_(0)
  , error_waits_(0)
  , generic_waits_(0)
  , canceled_(0)
{ }

core &
core::add(task & t) {
  scheduler::instance().add(t);
  return *this;
}

core &
core::own(task * t) {
  scheduler::instance().own(t);
  return *this;
}

void
core::react() {
  queue & q = queue::instance();
  reactor & r = reactor::instance();
  scheduler & s = scheduler::instance();

  while (!s.done()) {
    s.run();
    if (!s.active())
      passive_tasks.killall();
    if (r.armed())
      events_ += q.empty() ? r.wait(q) : r.poll(q);
    if (!q.empty())
      s.transfer(q);
  }
}

unsigned core::events() const { return events_; }
unsigned core::schedules() const { return schedules_; }
unsigned core::read_waits() const { return read_waits_; }
unsigned core::write_waits() const { return write_waits_; }
unsigned core::error_waits() const { return error_waits_; }

unsigned core::generic_waits() const { return generic_waits_; }
unsigned core::canceled() const { return canceled_; }

void
core::schedule() {
  ++core::instance().schedules_;

  ucontext_t c;
  queue::instance().enqueue(&c);
  scheduler::instance().defer(&c);
}

void
core::wait_for_read(trigger & t) {
  ++core::instance().read_waits_;
  scheduler::instance().defer(t.wait_for_read());
}

void
core::wait_for_write(trigger & t) {
  ++core::instance().write_waits_;
  scheduler::instance().defer(t.wait_for_write());
}

void
core::wait_for_error(trigger & t) {
  ++core::instance().error_waits_;
  scheduler::instance().defer(t.wait_for_error());
}

void
core::wait_for_nothing(trigger & t) {
  ++core::instance().canceled_;
  scheduler::instance().refer(t.wait_for_nothing());
}

void
core::wait_for_anything(trigger & t) {
  ++core::instance().generic_waits_;
  scheduler::instance().defer(t.wait_for_anything());
}

size_t
core::read(trigger & t, void * p, size_t min, size_t max) {
  assert(p);
  assert(min);
  assert(min <= max);

  size_t total = 0;
  char * buf = static_cast<char *>(p);

  while (ssize_t n = ::read(t.fd(), buf, max)) {
    if (n < 0)
    switch (errno) {
    case EAGAIN:
      wait_for_read(t);
      continue;

    case EINTR:
      continue;

    default:
      return total;
    }

    total += n;
    if (min <= total)
      break;

    buf += n;
    max -= n;
  }

  return total;
}

size_t
core::write(trigger & t, const void * p, size_t min, size_t max) {
  assert(p);
  assert(min);
  assert(min <= max);

  size_t total = 0;
  const char * buf = static_cast<const char *>(p);

  signal_suppressor ss(SIGPIPE);

  while (ssize_t n = ::write(t.fd(), buf, max)) {
    if (n < 0)
    switch (errno) {
    case EAGAIN:
      ss.restore();
      wait_for_write(t);
      ss.suppress();
      continue;

    case EINTR:
      continue;

    default:
      return total;
    }

    total += n;
    if (min <= total)
      break;

    buf += n;
    max -= n;
  }

  return total;
}

bool
core::shutdown(trigger & t, bool read, bool write, bool flush) {
  assert(file_descriptor::is_socket(t.fd()));
  assert(read || write);

  int how =  read &&  write ? SHUT_RDWR
          :  read && !write ? SHUT_RD
          : !read &&  write ? SHUT_WR
          : -1;

  while (!TRY_ERR(ENOTCONN, ::shutdown, t.fd(), how)) {
    if (!flush)
      break;
    wait_for_error(t);
  }

  return errno == ENOTCONN;
}

bool
core::become(const char * whom) {
  char buf[65536];
  struct passwd p, *r;
  TRY(getpwnam_r, whom, &p, buf, sizeof buf, &r);
  if (r != &p)
    return false;

  uid_t u = getuid();
  gid_t g = getgid();

  TRY(setgid, p.pw_gid);
  TRY(initgroups, whom, p.pw_gid);
  TRY(setuid, p.pw_uid);

  return setgid(g) < 0 && setuid(u) < 0;
}

void
core::pacify(task & t) {
  scheduler::instance().eye(t);
}

//
