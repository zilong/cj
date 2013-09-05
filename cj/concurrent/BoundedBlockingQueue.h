// Use of this source code is governed by a Apache license
// that can be found in the License file.
//

#ifndef MOMO_BASE_BOUNDEDBLOCKINGQUEUE_H
#define MOMO_BASE_BOUNDEDBLOCKINGQUEUE_H

#include "Condition.h"
#include "Mutex.h"

#include <boost/circular_buffer.hpp>
#include <boost/noncopyable.hpp>
#include <assert.h>

namespace concurrent
{

template<typename T>
class BoundedBlockingQueue : boost::noncopyable
{
 public:
  explicit BoundedBlockingQueue(int maxSize)
    : mutex_(),
      notEmpty_(mutex_),
      notFull_(mutex_),
      queue_(maxSize)
  {
  }

  void put(const T& x)
  {
    {
      MutexLockGuard lock(mutex_);
      while (queue_.full())
      {
        notFull_.wait();
      }
      assert(!queue_.full());
      queue_.push_back(x);
    }
    notEmpty_.notify();
  }

  T take()
  {
    {
      MutexLockGuard lock(mutex_);
      while (queue_.empty())
      {
        notEmpty_.wait();
      }
      assert(!queue_.empty());
      T front(queue_.front());
      queue_.pop_front();
      }
    notFull_.notify();
    return front;
  }

  bool empty() const
  {
    MutexLockGuard lock(mutex_);
    return queue_.empty();
  }

  bool full() const
  {
    MutexLockGuard lock(mutex_);
    return queue_.full();
  }

  size_t size() const
  {
    MutexLockGuard lock(mutex_);
    return queue_.size();
  }

  size_t capacity() const
  {
    MutexLockGuard lock(mutex_);
    return queue_.capacity();
  }

 private:
  mutable MutexLock          mutex_;
  Condition                  notEmpty_;
  Condition                  notFull_;
  boost::circular_buffer<T>  queue_;
};

}

#endif  // MOMO_BASE_BOUNDEDBLOCKINGQUEUE_H
