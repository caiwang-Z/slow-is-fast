#include <iostream>
#include "utility.h"

namespace TestMySmartPointer {
	// TOPIC: Introduction To Smart Pointers

	// NOTES:
	// 0. Smart pointer is a class which wraps a raw pointer, to manage the life time of the pointer.
	// 1. The most fundamental job of smart pointer is to remove the chances of memory leak.
	// 2. It makes sure that the object is deleted if it is not reference any more.

	// TYPES:
	// 1. unique_ptr : 
	//    Allows only one owner of the underlying pointer.

	// 2. shared_ptr : 
	//    Allows multiple owners of the same pointer (Reference count is maintained).

	// 3. weak_ptr : 
	//    It is special type of shared_ptr which doesn't count the reference.


	class MyInt {
	public:
		explicit MyInt(int* data) : _data(data)  {};
		~MyInt() { 
			delete _data;
			std::cout << "raw pointer has been freed.\n";
		};

		int& operator * () {
			return *_data;
		}

	private:
		int* _data;
	};

	void test() {
		int* intPtr = new int(3);
		MyInt smPtr(intPtr);
		//MyInt smPtr = intPtr;  // without explict in MyInt constructor it will work.

		std::cout << "Value of smart pointer is : " << *smPtr << std::endl;
		
	}

}

namespace TestUniquePointerBasic {
	// TOPIC: Unique Pointer In C++

	// NOTES:
	// 0. unique_ptr is a class template.
	// 1. unique_ptr is one of the smart pointer provided by c++11 to prevent memory leaks.
	// 2. unique_ptr wraps a raw pointer in it, and de-allocates the raw pointer,
	//    when unique_ptr object goes out of scope.
	// 3. similar to actual pointers we can use -> and * on the object of unique_ptr, 
	//    because it is overloaded in unique_ptr class.
	// 4. When exception comes then also it will de-allocate the memory hence no memory leak.
	// 5. Not only object we can create array of objects of unique_ptr.

	// OPERATIONS:
	// release: returns a pointer to the managed object and releases the ownership
	// reset: 	replaces the managed object
	// swap: swaps the managed objects
	// get: returns a pointer to the managed object
	// , get_deleter: returns the deleter that is used for destruction of the managed object
	
	using LogNameSpace::myLog;

	class Dummy {
	public:
		explicit Dummy(int val) : _data(val) {};
		~Dummy() {
			std::cout << "Current value: " << _data << ", destructor called.\n";
		}
		int getValue() { return _data; };

	private:
		int _data;

	};
	
	void testConstructUniquePointer() {
		std::unique_ptr<Dummy> intP1 = std::make_unique<Dummy>(100);
		myLog(intP1->getValue());

		std::unique_ptr<Dummy> intP2(new Dummy(99));
		myLog(intP2->getValue());
	
	}

	void testUniqueOwership() {
		std::unique_ptr<Dummy> intP1 = std::make_unique<Dummy>(100);
		std::unique_ptr<Dummy> intP3;
		//intP3 = intP1; // compile error

		Dummy* dy = new Dummy(999);
		std::unique_ptr<Dummy> intP5(dy);
		std::unique_ptr<Dummy> intP6(dy);
		myLog(intP5->getValue());
		//myLog(intP6->getValue());  // runtime error, destructor of Dummy will be called twice

	}

	void testGetFunction() {
		std::unique_ptr<Dummy> intP1 = std::make_unique<Dummy>(100);

		Dummy* dy = intP1.get();
		myLog(dy->getValue());
		if (intP1) {
			myLog(intP1->getValue());
		}
	}

	void testReleaseFunction() {
		std::unique_ptr<Dummy> intP1 = std::make_unique<Dummy>(100);

		Dummy* dy = intP1.release();
		myLog(dy->getValue());
		if (intP1) {
			myLog(intP1->getValue());
		}
		delete dy; // intP1 becomes nullptr, manually manage the raw pointer
	
	}

	//void testGetDeleterFunction() {
	//	std::unique_ptr<Dummy> intP1 = std::make_unique<Dummy>(100);

	//	Dummy* dy = intP1.release();
	//	myLog(dy->getValue());
	//	auto func = intP1.get_deleter();
	//	func();
	//
	//}

	void test() {
		//testConstructUniquePointer();
		//testUniqueOwership();
		//testGetFunction();
		testReleaseFunction();
	}
}



void test() {
	TestUniquePointerBasic::test();

	//TestMySmartPointer::test();

}