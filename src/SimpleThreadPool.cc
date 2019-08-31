#include "SimpleThreadPool.h"

namespace TinyToy{
    
SimpleThreadPool::SimpleThreadPool(int NumThread){
  Stop_ = false;
  size_ = NumThread;
  for(int i = 0; i < NumThread; i++){
    workers_.emplace_back(new std::thread([this](){
      std::function<void()> task;
      while(true){
        {
          std::unique_lock<std::mutex> lock(this->mutex_);
          this->cv_.wait(lock, [this](){return this->Stop_ || this->taskQueue_.empty();});
          if (this->Stop_ && this->taskQueue_.empty()){
            return;
          }
          task = std::move(this->taskQueue_.front());
          this->taskQueue_.pop();
        }
        task();
      }
    }));
  }
}

SimpleThreadPool::~SimpleThreadPool(){
  Stop_ = true;
  cv_.notify_all();
  for(int i = 0; i < size_ ;i++){
    if (workers_[i]->joinable())
      workers_[i]->join();
  }
}

void SimpleThreadPool::Schedule(std::function<void()> task){
  if (Stop_){
      std::cout << "ThreadPool Stoped" << std::endl;
      return;
  }
  std::unique_lock<std::mutex> lock(mutex_);
  taskQueue_.push(task);
  cv_.notify_one();
}

} //namespace TinyToy