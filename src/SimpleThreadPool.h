#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <atomic>
#include <condition_variable>

namespace TinyToy{

class SimpleThreadPool{
public:

  SimpleThreadPool(int NumThread);
  ~SimpleThreadPool();

  void Schedule(std::function<void()> task);

private:
  std::mutex mutex_;
  std::condition_variable cv_;

  std::atomic<bool> Stop_;

  int size_;
  std::vector<std::thread*> workers_;
  std::queue<std::function<void()>> taskQueue_;
};
  
} //namespace TinyToy