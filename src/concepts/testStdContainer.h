#pragma once
#include <array>

namespace TestStaticArray {
	void test() {
		std::array<int, 5> data{ 1,2,3,4,5 };
		const auto size = data.size();

		data[0] = 1;
	}
}

void test() {

}