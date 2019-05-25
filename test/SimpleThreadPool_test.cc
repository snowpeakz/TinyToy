#include <iostream>
#include "SimpleThreadPool.h"

using namespace std;

int main() {
	TinyToy::SimpleThreadPool ThreadPool(2);
	auto task1 = []() {
		std::this_thread::sleep_for(chrono::seconds(10));
		cout << "hello world" << endl; 
	};
	auto task2 = []() {cout << "i am TinyToy" << endl; };
	ThreadPool.Schedule(task1);
	ThreadPool.Schedule(task2);
	return 0;
}