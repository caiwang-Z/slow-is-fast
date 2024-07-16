#include <iostream>
#include "utility.h"
#include <functional>
#include <cassert>
#include <vector>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <format>
#include <execution>
#include <random>

using UtilityNameSpace::myLog;
using UtilityNameSpace::splitLine;
using UtilityNameSpace::Splitter;

//#define PARALLEL


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
			// std::cout << "x: " << x << "\n";  // compiler complain, cause the underlying value has been moved
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

	namespace TestBindFrontInMemberFunction {
		class Printer {
		public:
			Printer() = default;

			void print(const std::string& message) {
				std::cout << message << std::endl;
			}

		};

		void test() {
			Printer pr;
			// bind an object pointer (or reference) to a member function.
			auto newProcess = std::bind_front(&Printer::print, pr);

			newProcess("Hello World\n");
		}
	
	}

	void test() {
		TestBindFrontInMemberFunction::test();
	}
}

namespace TestSTDTransform {
	// std::transform applies the given function to the elements of the given input range(s), 
	// and stores the result in an output range starting from d_first.

	void test() {
		std::vector<int> vec{ 1,2,3, 4, 5, 6 };
		std::vector<int> dst(vec.size() );

		std::transform(vec.cbegin(), vec.cend(), dst.begin(), [](int x) {return x * x; });
		for (int i = 0; i < vec.size(); ++i) {
			myLog(vec[i]);
		}
 	}

}

namespace TestSTDForEach {
	// Applies the given function object f to the result of dereferencing every iterator in the range [first, last). 
	// If f returns a result, the result is ignored.
	void test() {
		std::vector<int> vec{ 1,2,3, 4, 5, 6 };
		std::for_each(vec.begin(), vec.end(), [](int& x) {x = x * 2; });
		for (int i = 0; i < vec.size(); ++i) {
			myLog(vec[i]);
		}
		splitLine();
		std::vector<int> vec1{ 10,20,30, 40, 50, 60 };
		std::for_each(vec1.begin(), vec1.end(), [](int x) {x = x * 2; });
		for (int i = 0; i < vec1.size(); ++i) {
			myLog(vec1[i]);
		}
	}

}

namespace TestStdRanges {
	void testStdViewsFilter() {
		std::vector<int> numbers{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		auto even_numbers = numbers | std::views::filter([](int n) {return n % 2 == 0; });
		for (int n : even_numbers) {
			std::cout << n << " ";
		}
		std::cout << "\n";  // 2 4 6 8 10

		auto even_numbers2 = std::views::filter(numbers, [](int n) {return n % 2 == 0; });
		for (int n : even_numbers2) {
			std::cout << n << " ";
		}
		std::cout << "\n";  // 2 4 6 8 10

		for (int n : numbers) {
			std::cout << n << " ";
		}
		std::cout << "\n";  // 1 2 3 4 5 6 7 8 9 10
	}

	void testStdViewsFilterByReference() {
		std::vector<int> numbers{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		auto even_numbers = numbers | std::views::filter([](int& n) {return n % 2 == 0; });
		for (int n : even_numbers) {
			std::cout << n << " ";
		}
		std::cout << "\n";  // 2 4 6 8 10

		for (int n : numbers) {
			std::cout << n << " ";
		}
		std::cout << "\n";  // 1 2 3 4 5 6 7 8 9 10
	}

	void testStdViewsTransform() {
		std::vector<int> numbers{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		auto square_numbers = numbers | std::views::transform([](int n) {return n * n; });
		auto square_numbers2 = std::views::transform(numbers, [](int n) {return n * n; });

		for (int n : square_numbers) {
			std::cout << n << " ";
		}
		std::cout << "\n";  // 1 4 9 16 25 36 49 64 81 100

		for (int n : square_numbers2) {
			std::cout << n << " ";
		}
		std::cout << "\n";  // 1 4 9 16 25 36 49 64 81 100

		for (int n : numbers) {
			std::cout << n << " ";
		}
		std::cout << "\n";  // 1 2 3 4 5 6 7 8 9 10
	}

	void testStdRangeAdapter() {
		std::vector<int> numbers{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		auto res = numbers | std::views::filter([](int n) {return n % 2 == 1; }) | std::views::transform([](int n) {return n * n; });
		for (int n : res) {
			std::cout << n << " ";
		}
		std::cout << "\n";  // 1 9 25 49 81

		for (int n : numbers) {
			std::cout << n << " ";
		}
		std::cout << "\n";  // 1 2 3 4 5 6 7 8 9 10
	
	}

	void testStdRangesForEach() {
		std::vector<int> numbers{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		std::ranges::for_each(numbers, [](int n) {n *= n; });

		for (int n : numbers) {
			std::cout << n << " ";
		}
		std::cout << "\n";  // 1 2 3 4 5 6 7 8 9 10

		std::ranges::for_each(numbers, [](int& n) {n *= n; });

		for (int n : numbers) {
			std::cout << n << " ";
		}
		std::cout << "\n";  // 1 4 9 16 25 36 49 64 81 100
	}

	void testStdRangesCount() {
		std::vector<int> numbers{ 1, 2, 3, 4, 5, 3, 7, 8, 3, 10 };
		const auto res = std::ranges::count(numbers, 3); 
		std::cout << res << std::endl; // 3
	}

	void testStdRangesCountIf() {
		std::vector<int> numbers{ 1, 2, 3, 4, 5, 3, 7, 8, 3, 10 };
		const auto res = std::ranges::count_if(numbers, [](int n) {return n >= 4; });
		std::cout << res << std::endl; // 5
	
	}
	
	
	void test() {
		testStdViewsFilter();
		splitLine();
		testStdViewsFilterByReference();
		splitLine();
		testStdViewsTransform();
		splitLine();
		testStdRangeAdapter();
		splitLine();
		testStdRangesForEach();
		splitLine();
		testStdRangesCount();
		splitLine();
		testStdRangesCountIf();
	}

}

namespace TestSTDDistance {
	void test() {
		std::vector<int> vec{ 3, 1, 9, 8 };
		myLog(std::distance(vec.cbegin(), vec.cend())); // 4
		myLog(std::distance(vec.cend(), vec.cbegin())); // -4


	
	}

}

namespace TestSTDFindIf {
	void testSTDFind() {
		std::vector<int> vec{ 3, 1, 9, 8 };
		if (std::find(vec.cbegin(), vec.cend(), 9) != vec.cend()) {
			myLog("Found value.\n");
		}
		else {
			myLog("Vec does not contain 9");
		}
	
	}

	void testSTDFindIf() {
		std::vector<int> vec{ 3, 1, 9, 8 };
		if (std::find_if(vec.cbegin(), vec.cend(), [](auto& val) {
			return val == 9;
			}) != vec.cend()) {
			myLog("Found value.\n");
		}
		else {
			myLog("Vec does not contain 9");
		}
	
	}

	void test() {
		testSTDFind();
		splitLine();
		testSTDFindIf();
	}

}

namespace TestStdStringView {

	void testStringViewBasic() {
		const char* str = "this is a string";
		std::string_view sv1(str);
		myLog(sv1);
		splitLine();
		std::string_view sv2(str, 6);
		myLog(sv2);
	
	}
}

namespace TestStdRefBasic {

	void foo(int& n1, int& n2, const int& n3) {
		//std::cout << "In foo: n1: " << n1 << ", n2: " << n2 << ", n3: " << n3 << "\n";
		++n1;
		++n2;
		// ++n3  // compile error
	
	}

	void test() {
		int a1 = 1, a2 = 2, a3 = 3;
		std::function<void()> new_foo = std::bind(foo, a1, std::ref(a2), std::cref(a3));
		a1 = 10;
		a2 = 20;
		a3 = 30;
		std::cout << "Before calling: a1: " << a1 << ", a2: " << a2 << ", a3: " << a3 << "\n";  // 10 20 30
		new_foo();
		std::cout << "After calling: a1: " << a1 << ", a2: " << a2 << ", a3: " << a3 << "\n";  // 10 21 30

	
	}

}

namespace TestStdIota {
	// include <numberic>
// Fills the range [first, last) with sequentially increasing values, 
// starting with value and repetitively evaluating ++value.
	void test() {

		std::vector<int> vec(10);
		std::iota(vec.begin(), vec.end(), 10);
		std::for_each(vec.cbegin(), vec.cend(), [](int x) {myLog(std::format(" {}", x)); }); // 10 11 12 13 14 15 16 ... 19
	
	}

}

namespace TestStdExecution {

	// include <execution>
#ifdef PARALLEL
#include <execution>
	namespace execution = std::execution;
#else
	enum class execution { seq, unseq, par_unseq, par };
#endif

	void measure([[maybe_unused]] auto policy, std::vector<std::uint64_t> v)
	{
		const auto start = std::chrono::steady_clock::now();
#ifdef PARALLEL
		std::sort(policy, v.begin(), v.end());
#else
		std::sort(v.begin(), v.end());
#endif
		const auto finish = std::chrono::steady_clock::now();
		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << '\n';
	};
	void testExecution() {
		SIFSplit;
		std::vector<std::uint64_t> v(1'000'000);
		std::mt19937 gen{ std::random_device{}() };
		std::ranges::generate(v, gen);

		measure(execution::seq, v);
		measure(execution::unseq, v);
		measure(execution::par_unseq, v);  // no parallel: 78ms, with parallel: 32ms
		measure(execution::par, v);  // no parallel: 81ms, with parallel: 37ms
	
	}
}

namespace TestStdErase {
	void test() {
		std::vector<int> list{ 1, 2, 3, 2, 4 };
		std::erase(list, 2);  // 1, 3, 4
		[[maybe_unused]] int a = 1;
	}

}

namespace TestStdRemove {
	/*
	std::remove doesn't actually remove the elements, it moves forward all elements not equal to the given value and returns a new end iterator that points to the new end after the "remove operation".
	It doesn't change the size of the container, it just rearranges the elements.
	
	erase will actually remove the element and resize the container.
	erase takes two arguments indicating the range to be removed: from a start iterator to an end iterator (excluding the end iterator).

	*/
	void test() {
		std::vector<int> list{ 1, 2, 3, 2, 4 };
		list.erase(
			std::remove(list.begin(), list.end(), 3), list.end()
		); // 1, 2, 2, 4

		[[maybe_unused]] int a = 1;
	}

}

namespace TestStdMinElement {
	/*
		returns the smallest element in a range

	*/
	std::vector<int> v{ 3, 1, -4, 1, 5, 9 };

	void test() {
		const auto it = std::min_element(v.begin(), v.end());
		std::cout << "Value of smallest element: " << *it << " , index: " << std::distance(it, v.begin()) << " , index in reverse order: " << std::distance(v.begin(), it);
		// -4, -2, 2
	
	}

}

namespace TestStdMaxElement {
/*
	returns the largest element in a range
*/
	std::vector<int> v{ 3, 1, -4, 1, 5, 9 };

	void test() {
		const auto it = std::max_element(v.begin(), v.end());
		std::cout << "Value of smallest element: " << *it << " , index: " << std::distance(it, v.begin()) << " , index in reverse order: " << std::distance(v.begin(), it);
		// 9, -5, 5

	}

}

namespace TestStdExchange{
/*
	template< class T, class U = T >
	T exchange( T& obj, U&& new_value );
	Replaces the value of obj with new_value and returns the old value of obj.
*/
namespace UseCase1{
	// Resetting the value of a variable
	void resetValue(int& value){
		const int oldValue = std::exchange(value, 0); 
		std::cout << "Old value: " << oldValue << std::endl; // 42
	
	}

	void test(){
		int num = 42;
		resetValue(num);
		std::cout << "new value: " << num << std::endl; // 0
	
	}

}

namespace UseCase2{
	// simplify swaping operation
	void test(){
		int x = 5, y = 10;
		std::cout << "x: " << x << ", y: " << y << std::endl;  // 5 10
		y = std::exchange(x, y);
		std::cout << "x: " << x << ", y: " << y << std::endl;  // 10 5

	}

}

namespace UseCase3{
	// State transitions using std::exchange
	enum class State {
		Idle,
		Running,
		Stopped

	};

	class Machine {
		public:
			State changeState(State sta){
				return std::exchange(_curState, sta);
			}

			State getState(){
				return _curState;
			}

		private:
			State _curState = State::Idle;


	};

	void test(){
		Machine machine;
		std::cout << "Old state: " << static_cast<int>(machine.changeState(State::Running)) << std::endl;  // 0 (Idle)
		std::cout << "New state: " << static_cast<int>(machine.getState()) << std::endl;  // 1 (Running)
	}
}

    namespace UseCase4{
		// Resource management and movement operations
		class Resource {
			public:
				Resource(int value) : _data(new int(value)){}

				// copy constructor
				Resource(const Resource& other): _data(new int(*other._data)){
				}

				// move constructor
				Resource(Resource&& other) noexcept : _data(std::exchange(other._data, nullptr)){}

				// move assignment operator
				Resource& operator= (Resource&& other) noexcept {
					if (this != &other){
						delete _data;
						_data = std::exchange(other._data, nullptr);
					}
					return  *this; 

				}

				// copy assignment operator
				Resource& operator= (Resource& other) noexcept{
					if (this != &other){
						delete _data;
						_data = new int(*other._data);
					
					}
					return *this;
				}

				~Resource(){delete _data;}

				int getValue(){
					return _data ? *_data : 0;
				}

			private:

				int* _data;


		};

		void test(){
			Resource res1(42);
			Resource res2(std::move(res1)); // call move constructor
			//Resource res2 = std::move(res2); // call move constructor
			std::cout << "res1 value: " << res1.getValue() << std::endl; // 0
			std::cout << "res2 value: " << res2.getValue() << std::endl; // 42

			Resource res3(100);
			res3 = std::move(res2);  // call move assignment operator
			std::cout << "res2 value: " << res2.getValue() << std::endl; // 0
			std::cout << "res3 value: " << res3.getValue() << std::endl; // 42

		}
	
	
	}
    void test(){
		UseCase1::test();
		UseCase2::test();
		UseCase3::test();
		UseCase4::test();
	}

}

void test() {
	TestStdExchange::test();

	//TestStdMinElement::test();

	//TestStdErase::test();
	//TestStdRemove::test();

	//TestStdExecution::testExecution();
	//TestStdIota::test();
	//TestStdRefBasic::test();
	//TestSTDBindBasic::test();
	//TestStdStringView::testStringViewBasic();
	//TestSTDDistance::test();
	//TestSTDFindIf::test();
	//TestSTDForEach::test();
	//TestStdRanges::test();
	//TestSTDForEach::test();
	//TestSTDTransform::test();
	//TestSTDForward::TestSTDForwardWithinClass::test();
	//TestSTDForward::TestLRValueWithinClass::test();
	//TestSTDForward::testLRvalueBasic();
	//TestSTDFunctionBasic::test();
	//TestSTDBindBasic::testBind();

}