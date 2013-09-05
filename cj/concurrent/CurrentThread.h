// Use of this source code is governed by a Apache license
// that can be found in the License file.
//

#ifndef MOMO_BASE_CURRENTTHREAD_H
#define MOMO_BASE_CURRENTTHREAD_H

namespace concurrent
{
namespace CurrentThread
{
  // internal
  extern __thread int t_cachedTid;
  extern __thread char t_tidString[32];
  extern __thread const char* t_threadName;
  void cacheTid();

  inline int tid()
  {
    if (t_cachedTid == 0)
    {
      cacheTid();
    }
    return t_cachedTid;
  }

  inline const char* tidString() // for logging
  {
    return t_tidString;
  }

  inline const char* name()
  {
    return t_threadName;
  }

  bool isMainThread();
}
}

#endif  //MOMO_BASE_CURRENTTHREAD_H
