#include "Allocator.h"
#include <iostream>

int main() {
	TinyToy::Allocator a;
	auto p1 = a.allocate(20);
	std::cout << "p1 = " << p1 << std::endl;
	auto p2 = a.allocate(20);
	std::cout << "p2 = " << p2 << std::endl;
	a.relase(p1);
	a.relase(p2);
	return 0;