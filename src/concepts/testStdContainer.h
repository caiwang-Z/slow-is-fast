#pragma once
#include <array>
#include <iostream>
namespace TestStaticArray {
	void test() {
		std::array<int, 5> data{ 1,2,3,4,5 };
		const auto size = data.size();

		data[0] = 1;
	}
}

// test for static array
namespace TestStaticArrayData {
	struct imageInfo {
		int width;
		int height;
		int linePitch;
	};

	void test() {
		std::array<imageInfo, 3> infos{ {
			{100, 200, 300},
			{99, 299, 399},
			{188, 288, 388}
		} };
		imageInfo** info = new imageInfo*;
		*info = infos.data();
		std::cout << ((*info) + 1)->linePitch << " " << ((*info) + 2)->height << std::endl;

	}
}

void test() {

}