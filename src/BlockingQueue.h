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

bool offer(T &item);
void put(T &item);

bool poll(T *item);
void takeOne(T *item);
int takeSome(std::vector<T*> *items, int num);
int takeAll(std::vector<T*> *items);

private:
std::queue queue_;
std::mutex mu_;
std::condition_variable cv_produ;
std::condition_variable cv_consu;

};

} //namespace TinyToy

#endif //TINYTOY_BLOCKINGQUEUE_H