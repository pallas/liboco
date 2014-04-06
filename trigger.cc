#include "trigger.h"

#include "try.h"
#include "reactor.h"

#include <cassert>
#include <unistd.h>

trigger::trigger(int fd)
  : fd_(fd)
{
  uc_link = NULL;
  ev_.events = EPOLLONESHOT;
  ev_.data.ptr = NULL;
  TRY_ERR(EPERM, epoll_ctl, reactor::instance().fd, EPOLL_CTL_ADD, fd_, &ev_);
  ++reactor::instance().total_triggers;
}

trigger::trigger(const file_descriptor & fd)
  : fd_(fd)
{
  uc_link = NULL;
  ev_.events = EPOLLONESHOT;
  ev_.data.ptr = NULL;
  TRY_ERR(EPERM, epoll_ctl, reactor::instance().fd, EPOLL_CTL_ADD, fd_, &ev_);
  ++reactor::instance().total_triggers;
}

trigger::~trigger() {
  TRY_ERR(EPERM, epoll_ctl, reactor::instance().fd, EPOLL_CTL_DEL, fd_, &ev_);
  --reactor::instance().total_triggers;
}

int trigger::fd() const { return fd_; }
int trigger::dup() const { return file_descriptor::dup(fd_); }

ucontext_t *
trigger::wait_for_read() {
  if (!armed())
    arm();
  ev_.events = EPOLLRDHUP|EPOLLIN|EPOLLONESHOT;
  TRY(epoll_ctl, reactor::instance().fd, EPOLL_CTL_MOD, fd_, &ev_);
  return static_cast<ucontext_t*>(this);
}

ucontext_t *
trigger::wait_for_write() {
  if (!armed())
    arm();
  ev_.events = EPOLLOUT|EPOLLONESHOT;
  TRY(epoll_ctl, reactor::instance().fd, EPOLL_CTL_MOD, fd_, &ev_);
  return static_cast<ucontext_t*>(this);
}

ucontext_t *
trigger::wait_for_error() {
  if (!armed())
    arm();
  ev_.events = EPOLLRDHUP|EPOLLONESHOT;
  TRY(epoll_ctl, reactor::instance().fd, EPOLL_CTL_MOD, fd_, &ev_);
  return static_cast<ucontext_t*>(this);
}

ucontext_t *
trigger::wait_for_nothing() {
  if (armed())
    disarm();
  ev_.events = EPOLLONESHOT;
  TRY(epoll_ctl, reactor::instance().fd, EPOLL_CTL_MOD, fd_, &ev_);
  return static_cast<ucontext_t*>(this);
}

ucontext_t *
trigger::wait_for_anything() {
  if (!armed())
    arm();
  ev_.events = EPOLLRDHUP|EPOLLIN|EPOLLOUT|EPOLLONESHOT;
  TRY(epoll_ctl, reactor::instance().fd, EPOLL_CTL_MOD, fd_, &ev_);
  return static_cast<ucontext_t*>(this);
}

bool
trigger::readable() const {
  assert(!armed());
  return ev_.events & EPOLLIN;
}

bool
trigger::writable() const {
  assert(!armed());
  return ev_.events & EPOLLOUT;
}

ucontext_t *
trigger::fire() {
  assert(armed());
  disarm();
  return static_cast<ucontext_t*>(this);
}

void
trigger::arm() {
  assert(!armed());
  ev_.data.ptr = this;
  ++reactor::instance().armed_triggers;
  assert(armed());
}

void
trigger::disarm() {
  assert(armed());
  ev_.data.ptr = NULL;
  --reactor::instance().armed_triggers;
  assert(!armed());
}

bool
trigger::armed() const {
  return ev_.data.ptr == static_cast<const trigger*>(this);
}

//
