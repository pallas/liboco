liboco
======

liboco is a simple cooperative multitasking event framework written in C++
as an exercise for the author.  It is not meant as a replacement for
libpthread, libevent, libev, &c.

Based around ucontext and epoll, it is not currently intended to be
portable.  In fact, it uses glibc extensions and Linux-specific system
calls, so it will probably only compile with that standard library and run
on a very modern kernel.

It is also not very safe regarding signals that are not processed via
signalfd.  In particular, calling core::schedule or core::wait_for_* with
any pending signals that were not blocked when core::react was invoked are
likely to be handled at that time.  Signal masks should not be modified from
within tasks.

It comes with the following example programs.

 * rfc862d --- Echo server, bound to any IPv4 address on port 7.
 * rfc862c --- Echo client, connects to 127.0.0.1 on port 7.

and

 * open_fds --- Lists open FDs.  When launched as a child, useful for
   finding FD leaks in parent process.

While liboco is just a toy, the intention is to continue to add features and
hammer out some more interesting use-cases.
