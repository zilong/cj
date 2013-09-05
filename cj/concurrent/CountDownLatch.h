// Use of this source code is governed by a Apache license
// that can be found in the License file.
//

#ifndef MOMO_BASE_COUNTDOWNLATCH_H
#define MOMO_BASE_COUNTDOWNLATCH_H

#include "Condition.h"
#include "Mutex.h"

#include <boost/noncopyable.hpp>

namespace concurrent
{

class CountDownLatch : boost::noncopyable
{
 public:

  explicit CountDownLatch(int count);

  void wait();

  void countDown();

  int getCount() const;

 private:
  mutable MutexLock mutex_;
  Condition condition_;
  int count_;
};

}
#endif  // MOMO_BASE_COUNTDOWNLATCH_H
