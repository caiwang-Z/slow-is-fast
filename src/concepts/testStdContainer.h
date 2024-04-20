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
		size_t size() const { return S; };

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

namespace TestMyVector{

	template<typename T>	
	class Vector {
	public:
		Vector() {
			realloc(2);
		}

		void realloc(const size_t newCapacity) {

			const size_t copySize = (newCapacity >= _capacity ? _capacity : newCapacity);
			
			T* _newBlock = new T[newCapacity];
			//memcpy(_newBlock, _data, newCapacity);  // for primitiv type it works, but for classes, it won't work, cause constructor won't be called.
			for (size_t i = 0; i < copySize; ++i) {
				_newBlock[i] = _data[i];
			}

			_capacity = newCapacity;
			delete[] _data;
			_data = _newBlock;
		}

		void pushBack(const T& val) {
			if (_fillSize >= _capacity) {
				const auto newSize = _capacity + _capacity / 2;
				realloc(newSize);
				std::cout << "Reallocate a new vector with  " << newSize << " elements. \n";
			}
			_data[_fillSize] = val;
			_fillSize++;
		}

		size_t size() const {
			return _fillSize;
		}

		~Vector() {
			delete[] _data;
		}

		T& operator[] (size_t index) {
			return _data[index];
		}


		const T& operator[] (size_t index) const {
			return _data[index];
		}

	private:
		size_t _capacity{0};
		size_t _fillSize{ 0 };
		T* _data = nullptr;
	};

	template<typename T>
	void print(const Vector<T>& vec) {
		std::cout << "Vector values: ";
		for (size_t i = 0; i < vec.size(); ++i) {
			std::cout << vec[i] << " ";
		
		}
		std::cout << "\n";
	};

	void test() {
		Vector<int> vec;
		vec.pushBack(2);
		vec.pushBack(10);
		vec.pushBack(99);
		vec.pushBack(100);
		vec.pushBack(7);
		vec.pushBack(87);
		print(vec);
		
	}

	class Vector3 {

	public:
		Vector3() {};

		// scalar constructor
		Vector3(double x, double y, double z): _x(x), _y(y), _z(z) {
		}
		Vector3(double scalar): _x(scalar), _y(scalar), _z(scalar) {
		}

		// copy constructor
		Vector3(const Vector3& vec) {
			_x = vec._x;
			_y = vec._y;
			_z = vec._z;
		}

		// assignment operator
		Vector3& operator=(const Vector3& vec) {
			_x = vec._x;
			_y = vec._y;
			_z = vec._z;
			return *this;
		}


	private:
		double _x{ 0.0 }, _y{ 0.0 }, _z{ 0.0 };
	};

	void testAssignmentAndCopyConstructor() {
		// only call scalar constructor, would not call assignment operator.
		Vector3 vec = Vector3(4.0); 

		// The assignment operator would be used if the object was already constructed, 
		// and then a value was assigned to it later, such as:
		Vector3 vec1;
		vec1 = Vector3(4.0);

		// call copy constructor
		Vector3 vec2(vec1);
	}
}

void test() {
	//TestMyVector::test();
	TestMyVector::testAssignmentAndCopyConstructor();
}