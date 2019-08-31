#include <thread>
#include <iostream>
#include <chrono>
#include "BlockingQueue.h"

using namespace std;

int main() {
  TinyToy::BlockingQueue<int>* Que = new TinyToy::BlockingQueue<int>;
  auto task1 = [Que]() {
    for (int i = 0; i < 9; ++i) {
      Que->put(&i);
      std::cout << "put " << i << " into the Que" << std::endl;
      std::this_thread::sleep_for(chrono::seconds(1));
    }
  };
  auto task2 = [Que]() {
    for (int i = 9; i < 20; ++i) {
      int res = Que->offer(&i);
      std::cout << "offer " << i << " into the Que, res = " << res << std::endl;
      std::this_thread::sleep_for(chrono::seconds(1));
    }
  };
  auto task3 = [Que]() {
    for (int i = 0; i < 9; ++i) {
      int temp = -1;
      Que->takeOne(&temp);
      std::cout << "take " << temp << " out the Que" << std::endl;
      std::this_thread::sleep_for(chrono::seconds(1));
    }
  };
  auto task4 = [Que]() {
    for (int i = 0; i < 9; ++i) {
      int temp = -1;
      Que->poll(&temp);
      std::cout << "poll " << temp << " out the Que" << std::endl;
      std::this_thread::sleep_for(chrono::seconds(1));
    }
  };
  std::thread t1(task1);
  std::thread t2(task2);
  std::thread t3(task3);
  std::thread t4(task4);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  return 0;
}
