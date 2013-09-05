// Use of this source code is governed by a Apache license
// that can be found in the License file.
//

#ifndef MOMO_BASE_BLOCKINGQUEUE_H
#define MOMO_BASE_BLOCKINGQUEUE_H

#include "Condition.h"
#include "Mutex.h"

#include <boost/noncopyable.hpp>
#include <deque>
#include <assert.h>

namespace concurrent
{

template<typename T>
class BlockingQueue : boost::noncopyable
{
 public:
  BlockingQueue()
    : mutex_(),
      notEmpty_(mutex_),
      queue_()
  {
  }

  void put(const T& x)
  {
    {
        MutexLockGuard lock(mutex_);
        queue_.push_back(x);
    }
    notEmpty_.notify(); 
  }

  T take()
  {
    MutexLockGuard lock(mutex_);
    // always use a while-loop, due to spurious wakeup
    while (queue_.empty())
    {
      notEmpty_.wait();
    }
    assert(!queue_.empty());
    T front(queue_.front());
    queue_.pop_front();
    return front;
  }

  size_t size() const
  {
    MutexLockGuard lock(mutex_);
    return queue_.size();
  }

 private:
  mutable MutexLock mutex_;
  Condition         notEmpty_;
  std::deque<T>     queue_;
};

}

#endif  // MOMO_BASE_BLOCKINGQUEUE_H
