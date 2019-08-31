#ifndef TINYTOY_BLOCKINGQUEUE_H
#define TINYTOY_BLOCKINGQUEUE_H

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace TinyToy {

template<typename T>
class BlockingQueue {
public:
BlockingQueue();
~BlockingQueue();

bool offer(T* item);
void put(T* item);

bool poll(T* item);
void takeOne(T* item);
int takeAll(std::vector<T>* items);

private:
std::queue<T> queue_;
std::mutex mu_;
std::condition_variable cv_consu;

};

template<typename T>
BlockingQueue<T>::BlockingQueue() {}

template<typename T>
BlockingQueue<T>::~BlockingQueue() {}

template<typename T>
bool BlockingQueue<T>::offer(T* item) {
  if (mu_.try_lock() == false) {
    return false;
  }
  queue_.push(*item);
  mu_.unlock();
  return true;
}

template<typename T>
void BlockingQueue<T>::put(T* item) {
  std::lock_guard<std::mutex> lock(mu_);
  queue_.push(*item);
  cv_consu.notify_one();
  return;
}

template<typename T>
bool BlockingQueue<T>::poll(T* item) {
  if (mu_.try_lock() == false || queue_.size() == 0) {
    return false;
  }
  *item = queue_.front();
  queue_.pop();
  mu_.unlock();
  return true;
}

template<typename T>
void BlockingQueue<T>::takeOne(T* item) {
  std::unique_lock<std::mutex> lock(mu_);
  cv_consu.wait(lock);
  *item = queue_.front();
  queue_.pop();
  return;
}

template<typename T>
int BlockingQueue<T>::takeAll(std::vector<T>* items) {
  std::unique_lock<std::mutex> lock(mu_);
  cv_consu.wait(lock);
  int res = queue_.size();
  while (queue_.size() > 0) {
    (*items).push_back(queue_.front());
    queue_.pop();
  }
  return res;
}

} //namespace TinyToy

#endif //TINYTOY_BLOCKINGQUEUE_H