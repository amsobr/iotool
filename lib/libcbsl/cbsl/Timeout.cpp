/*
 * Timeout.cpp
 *
 *  Created on: 7 Aug 2014
 *      Author: edge
 */

#include "Timeout.h"
#include <errno.h>

namespace cbsl {

Timeout::Timeout(callback_function_t* func,
                 void* func_arg,
                 unsigned int sleep_periodmiliseconds)
        : is_running_(true),
          sleep_time_(),
          callback_(func),
          callback_arg_(func_arg)
{
  sleep_time_.tv_sec = sleep_periodmiliseconds / 1000;
  sleep_time_.tv_nsec = (sleep_periodmiliseconds - (sleep_time_.tv_sec * 1000))
          * 1000000;
}

Timeout::~Timeout()
{
  is_running_ = false;
}

void
Timeout::StartTimer()
{
  log_trace(logctx.main, "Timeout thread starting.\n");
  Thread::start();
}

void
Timeout::run()
{
  struct timespec remaining_time;

  log_trace(logctx.main, "Timeout thread running.\n");

  while (is_running_) {
    // if interrupted by signal then nanosleep returns remaining time.
    if (nanosleep(&sleep_time_, &remaining_time) == -1) {
      if (errno == EINTR) {
        nanosleep(&remaining_time, &remaining_time);
        continue;
      }
    }

    // call call-back
//    log_trace(logctx.main, "Timeout thread callback.\n");
    callback_(callback_arg_);
  }

  log_trace(logctx.main, "Timeout thread terminating.\n");
}

} /* namespace Wcommon */
