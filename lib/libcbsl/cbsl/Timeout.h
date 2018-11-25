/*
 * Timeout.h
 *
 *  Created on: 7 Aug 2014
 *      Author: edge
 */

#ifndef TIMEOUT_H_
#define TIMEOUT_H_

#include "thread.hpp"
#include <time.h>

namespace cbsl {

class Timeout : public cbsl::Thread {
public:
  typedef void
  (callback_function_t)(void* arg);

  Timeout(callback_function_t* func,
          void* func_arg,
          unsigned int sleep_periodmiliseconds);
  virtual
  ~Timeout();

  void
  StartTimer();
private:
  bool is_running_;
  struct timespec sleep_time_;
  callback_function_t* callback_;
  void* callback_arg_;
  virtual void
  run();

  Timeout(Timeout const &);
  Timeout&
  operator=(Timeout const &);
};

} /* namespace Wcommon */

#endif /* TIMEOUT_H_ */
