#include "signal_killer.h"

signal_killer::signal_killer(const signal_set & ss) : signaler(ss) { }
void signal_killer::on_signal(const info_t &) { killer::killall(); }

//
