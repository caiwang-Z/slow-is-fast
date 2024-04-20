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

namespace TestStaticAndDynamicArrayBasic {
	void test() {
		int size = 5;
		// int stackArray[size]; // Error! allocate array in stack, size must be known in compile time

		int* heapArray = new int[size]; // Ok! Size of array in heap must not be known in compile time
		delete[] heapArray;

	}

}

namespace TestMyArray {

	template<typename T, size_t S>
	class Array {
	public:
		size_t size() const { return S };

		T& operator[](int index) {
			if (!(index < S)) {
				__debugbreak();
			}
			return _data[index];
		}

		T* data() { return _data; }

	private:
		T _data[S];
	};


	void test() {
		int size = 5;
		Array<int, 5> arr;
		arr.size();
		memset(arr.data(), 0, arr.size());
		const auto& arrayReference = arr;
		for (size_t i{ 0 }; i < arrayReference.size(); ++i) {
			arr[i] = 2;
		}
	}

}

void test() {

}