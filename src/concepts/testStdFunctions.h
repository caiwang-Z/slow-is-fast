#include <iostream>
#include "utility.h"
#include <functional>
#include <cassert>
#include <vector>


namespace TestStaticAssert {

	void test() {
		// performs runtime assertion checking
		assert(1 == 9);
	}

}

namespace TestAssert {

	void test() {
		// performs compile-time assertion checking
		static_assert(1 == 1);
	}

}

namespace TestStdGet {
// TODO test std::get


	void test() {
	}
}

// TODO std::forward
namespace TestSTDForward {
	void testLRvalueBasic() {
		{
			std::vector<std::string> vec;
			std::string x = "abcd";
			vec.push_back(x);
			std::cout << "x: " << x << "\n";
			std::cout << "vector: " << vec[0] << "\n";
		}
		{
			std::vector<std::string> vec;
			std::string x = "abcd";
			vec.push_back(std::move(x));
			std::cout << "x: " << x << "\n";
			std::cout << "vector: " << vec[0] << "\n";
		}
	}

	namespace TestLRValueWithinClass {
		class A {
		public:
			A() {}
			A(size_t size) : size(size), array((int*)malloc(size)) {
				std::cout
					<< "create Array. memory at: "
					<< array << std::endl;

			}
			~A() {
				free(array);
			}
			A(A&& a) : array(a.array), size(a.size) {
				a.array = nullptr;
				std::cout
					<< "Array moved. memory at: "
					<< array
					<< std::endl;
			}
			A(const A& a) : size(a.size) {
				array = (int*)malloc(a.size);
				for (int i = 0; i < a.size; i++)
					array[i] = a.array[i];
				std::cout
					<< "Array copied, memory at: "
					<< array << std::endl;
			}
			size_t size;
			int* array;
		};

		void test() {
			{
				std::vector<A> vec;
				A a = A(10);
				vec.push_back(a);
			}

			{
				std::vector<A> vec;
				A a = A(10);
				vec.push_back(std::move(a));
			}
		}
	}

	namespace TestSTDForwardWithinClass {
		/*
			T& & -> T& (a left-valued reference to a left-valued reference is a left-valued reference)
			T& && -> T& (a right-valued reference to a left-valued reference is a left-valued reference)
			T&& & -> T& (a left-valued reference to a right-valued reference is a left-valued reference)
			T&& && ->T&& (a right-valued reference to a right-valued reference is a right-valued reference)

			So an important role of std::forward is perfect forwarding, to ensure that the type of the reference does not change during the forwarding process, left-valued references must 
			still be left-valued references after forwarding, right-valued references must still be right-valued references after forwarding
		*/

		class A {
		public:
			A() {}
			A(size_t size) : size(size), array((int*)malloc(size)) {
				std::cout
					<< "create Array. memory at: "
					<< array << std::endl;

			}
			~A() {
				free(array);
			}
			A(A&& a) : array(a.array), size(a.size) {
				a.array = nullptr;
				std::cout
					<< "Array moved, memory at: "
					<< array
					<< std::endl;
			}
			A(const A& a) : size(a.size) {
				array = (int*)malloc(a.size);
				for (int i = 0; i < a.size; i++)
					array[i] = a.array[i];
				std::cout
					<< "Array copied, memory at: "
					<< array << std::endl;
			}
			size_t size;
			int* array;
		};

		template<typename T>
		void warp(T&& param) {
			if (std::is_rvalue_reference<decltype(param)>::value) {
				std::cout << "param is rvalue reference\n";
			}
			else std::cout << "param is lvalue reference\n";
			A y = A(param);  // copy constructor, rvalue refererence -> lvalue reference. T&& & -> T& not efficient.
			A z = A(std::forward<T>(param));  // move constructor, perfect forwarding, efficient
		}

		void test() {
			A a = A(100);
			warp(std::move(a));
		}
		
	}

}

namespace TestSTDFunctionBasic {
	void printNum(int i) {
		myLog(i);
	}

	class Foo {
	public:
		Foo(const std::string& name) : _name(name) {}
		void print(int age) {
			std::cout << "Name: " << _name << " age: " << age << "\n";
		}

	private:
		std::string _name;
	};

	void test() {
		//std::function<void(int)> f1 = printNum;  // old usage
		std::function<void(int)> f1 = &printNum;  // new usage
		f1(199);

		Foo foo{ "jerry" };
		std::function<void(Foo&, int)> f2 = &Foo::print;
		f2(foo, 100);

		std::function<void(int)> f3 = std::bind(&Foo::print, foo, std::placeholders::_1);
		f3(999);

	}

}


namespace TestSTDBindBasic {
	double myDivide(double x, double y) 
	{ 
		return x / y; 
	}

	void testBind() {
		auto f1 = std::bind(myDivide, 10.0, 2.0);
		myLog(f1());  // 10 / 2

		auto f2 = std::bind(myDivide, std::placeholders::_1, 2);
		myLog(f2(8));  // 8 / 2

		auto f3 = std::bind(myDivide, 6, std::placeholders::_1);
		myLog(f3(2));  // 6 / 2

		auto f4 = std::bind(myDivide, std::placeholders::_2, std::placeholders::_1);
		myLog(f4(2, 18));  // 18 / 2

		auto f5 = std::bind(myDivide, 6, std::placeholders::_2);
		myLog(f5(2, 1));  // 6 / 1

		// The std::bind_front function creates callable wrappers from callable objects. 
		// A call to std::bind_front(func, arg...) will bind all arguments arg to the front of func and return a callable wrapper
		auto f6 = std::bind_front(myDivide, 98);
		myLog(f6(2));  // 98 / 2

		// The std::bind_back function creates callable wrappers from callable objects. 
		// A call to std::bind_back(func, arg...) will bind all arguments arg to the back of func and return a callable wrapper.
		//auto f7 = std::bind_back(myDivide, 3);  // since c++23
		//myLog(f7(99));  // 99 / 3
	}
}


void test() {
	TestSTDForward::TestSTDForwardWithinClass::test();
	//TestSTDForward::TestLRValueWithinClass::test();
	//TestSTDForward::testLRvalueBasic();
	//TestSTDFunctionBasic::test();
	//TestSTDBindBasic::testBind();

}