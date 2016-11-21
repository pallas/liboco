#ifndef CORE_H
#define CORE_H

#include <lace/singleton.h>

#include "killer.h"

#include <sys/types.h>

class task;
class trigger;

class core : public lace::singleton<core> {

  friend class lace::singleton<core>;
  core();

public:

  core & add(task &);
  core & own(task *);

  template <class T>
  core & eye(T & t) {
    pacify(t);
    passive_tasks.target(t);
    return *this;
  }

  void react();

  unsigned events() const;
  unsigned schedules() const;

  unsigned read_waits() const;
  unsigned write_waits() const;
  unsigned error_waits() const;

  unsigned generic_waits() const;
  unsigned canceled() const;

  void schedule();

  static void wait_for_read(trigger &);
  static void wait_for_write(trigger &);
  static void wait_for_error(trigger &);

  static void wait_for_nothing(trigger &);
  static void wait_for_anything(trigger &);

  static size_t read(trigger &, void *, size_t min, size_t max);
  static size_t write(trigger &, const void *, size_t min, size_t max);

  static bool shutdown(trigger &, bool read, bool write, bool flush=true);

  static bool become(const char *);

private:
  unsigned events_;
  unsigned schedules_;
  unsigned read_waits_;
  unsigned write_waits_;
  unsigned error_waits_;
  unsigned generic_waits_;
  unsigned canceled_;

  void pacify(task &);
  killer passive_tasks;
};

#endif//CORE_H
