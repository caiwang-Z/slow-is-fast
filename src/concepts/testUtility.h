#include <iostream>

namespace TestConditionalAndActionBreakpoints {
	void test() {
		for (int i{ 0 }; i < 100; ++i) {
			std::cout << i << std::endl;
		}
		int a = 1;
	}
}

void test() {

}