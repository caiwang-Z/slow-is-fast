#pragma once
#include <iostream>
#include <vector>

namespace TestFunctionPointers {
	// functions are just cpu instructions and they are stored somewhere in our binary when we actually compile our code
	void func(int value) {
		std::cout << "Hello world: " << value << std::endl;
	}

	void test() {


		auto function = func; // works
		auto function2 = &func; // also works;
		void(*funcPtr3)(int) = func; // also works
		funcPtr3(100);
	}
}

namespace TestLambdaFunctions {
	// How to use and when to use lambda?
	// Answer: whenever you have a function pointer you can use a lambda in C++ 
	std::vector<int> values = { 1, 5, 4, 2, 3 };
	auto it = std::find_if(values.begin(), values.end(), [](int value) {return value > 3; });

}

void test() {

	TestFunctionPointers::test();
}