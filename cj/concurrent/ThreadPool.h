// Use of this source code is governed by a Apache license
// that can be found in the License file.
//

#ifndef MOMO_BASE_THREADPOOL_H
#define MOMO_BASE_THREADPOOL_H

#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"
#include "../base/Types.h"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <deque>

namespace concurrent
{

class ThreadPool : boost::noncopyable
{
 public:
  typedef boost::function<void ()> Task;

  explicit ThreadPool(const string& name = string());
  ~ThreadPool();

  void start(int numThreads);
  void stop();

  void run(const Task& f);

 private:
  void runInThread();
  Task take();

  MutexLock mutex_;
  Condition cond_;
  string name_;
  boost::ptr_vector<concurrent::Thread> threads_;
  std::deque<Task> queue_;
  bool running_;
};

}

#endif
