// Use of this source code is governed by a Apache license
// that can be found in the License file.
//

#include "Condition.h"

#include <errno.h>

// returns true if time out, false otherwise.
bool concurrent::Condition::waitForSeconds(int seconds)
{
  struct timespec abstime;
  clock_gettime(CLOCK_REALTIME, &abstime);
  abstime.tv_sec += seconds;
  return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
}

