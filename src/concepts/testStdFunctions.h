#pragma
#include <algorithm>
#include <any>
#include <cassert>
#include <execution>
#include <format>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <regex>
#include <sstream>
#include <vector>
#include "utility.h"

using UtilityNameSpace::myLog;
using UtilityNameSpace::splitLine;
using UtilityNameSpace::Splitter;

//#define PARALLEL

namespace TestStaticAssert {

void test() {
  // performs runtime assertion checking
  assert(1 == 9);
}

}  // namespace TestStaticAssert

namespace TestAssert {

void test() {
  // performs compile-time assertion checking
  static_assert(1 == 1);
}

}  // namespace TestAssert

namespace TestStdGet {
// TODO test std::get

void test() {}
}  // namespace TestStdGet

// TODO std::forward
namespace TestSTDForward {
void testLRvalueBasic() {
  {
    std::vector<std::string> vec;
    std::string              x = "abcd";
    vec.push_back(x);
    std::cout << "x: " << x << "\n";
    std::cout << "vector: " << vec[0] << "\n";
  }
  {
    std::vector<std::string> vec;
    std::string              x = "abcd";
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
    std::cout << "create Array. memory at: " << array << std::endl;
  }
  ~A() { free(array); }
  A(A&& a) : array(a.array), size(a.size) {
    a.array = nullptr;
    std::cout << "Array moved. memory at: " << array << std::endl;
  }
  A(const A& a) : size(a.size) {
    array = (int*)malloc(a.size);
    for (int i = 0; i < a.size; i++)
      array[i] = a.array[i];
    std::cout << "Array copied, memory at: " << array << std::endl;
  }
  size_t size;
  int*   array;
};

void test() {
  {
    std::vector<A> vec;
    A              a = A(10);
    vec.push_back(a);
  }

  {
    std::vector<A> vec;
    A              a = A(10);
    vec.push_back(std::move(a));
  }
}
}  // namespace TestLRValueWithinClass

namespace TestSTDForwardWithinClass {
/*
        T& & -> T& (a left-valued reference to a left-valued reference is a left-valued reference)
        T& && -> T& (a right-valued reference to a left-valued reference is a left-valued reference)
        T&& & -> T& (a left-valued reference to a right-valued reference is a left-valued reference)
        T&& && ->T&& (a right-valued reference to a right-valued reference is a right-valued reference)

        So an important role of std::forward is perfect forwarding, to ensure that the type of the reference does not
   change during the forwarding process, left-valued references must still be left-valued references after forwarding,
   right-valued references must still be right-valued references after forwarding
*/

class A {
  public:
  A() {}
  A(size_t size) : size(size), array((int*)malloc(size)) {
    std::cout << "create Array. memory at: " << array << std::endl;
  }
  ~A() { free(array); }
  A(A&& a) : array(a.array), size(a.size) {
    a.array = nullptr;
    std::cout << "Array moved, memory at: " << array << std::endl;
  }
  A(const A& a) : size(a.size) {
    array = (int*)malloc(a.size);
    for (int i = 0; i < a.size; i++)
      array[i] = a.array[i];
    std::cout << "Array copied, memory at: " << array << std::endl;
  }
  size_t size;
  int*   array;
};

template <typename T>
void warp(T&& param) {
  if (std::is_rvalue_reference<decltype(param)>::value) {
    std::cout << "param is rvalue reference\n";
  } else
    std::cout << "param is lvalue reference\n";
  A y = A(param);  // copy constructor, rvalue refererence -> lvalue reference. T&& & -> T& not efficient.
  A z = A(std::forward<T>(param));  // move constructor, perfect forwarding, efficient
}

void test() {
  A a = A(100);
  warp(std::move(a));
}

}  // namespace TestSTDForwardWithinClass

}  // namespace TestSTDForward

namespace TestSTDFunctionBasic {

void printNum(int i) {
  myLog(i);
}

class Foo {
  public:
  Foo(const std::string& name) : _name(name) {}
  void print(int age) { std::cout << "Name: " << _name << " age: " << age << "\n"; }

  private:
  std::string _name;
};

void test() {
  // std::function<void(int)> f1 = printNum;  // old usage
  std::function<void(int)> f1 = &printNum;  // new usage
  f1(199);

  Foo                            foo{"jerry"};
  std::function<void(Foo&, int)> f2 = &Foo::print;
  f2(foo, 100);

  std::function<void(int)> f3 = std::bind(&Foo::print, foo, std::placeholders::_1);
  f3(999);
}

}  // namespace TestSTDFunctionBasic

namespace TestSTDBindBasic {
double myDivide(double x, double y) {
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
  // A call to std::bind_front(func, arg...) will bind all arguments arg to the front of func and return a callable
  // wrapper
  auto f6 = std::bind_front(myDivide, 98);
  myLog(f6(2));  // 98 / 2

  // The std::bind_back function creates callable wrappers from callable objects.
  // A call to std::bind_back(func, arg...) will bind all arguments arg to the back of func and return a callable
  // wrapper.
  // auto f7 = std::bind_back(myDivide, 3);  // since c++23
  // myLog(f7(99));  // 99 / 3
}

namespace TestBindFrontInMemberFunction {
class Printer {
  public:
  Printer() = default;

  void print(const std::string& message) { std::cout << message << std::endl; }
};

void test() {
  Printer pr;
  // bind an object pointer (or reference) to a member function.
  auto newProcess = std::bind_front(&Printer::print, pr);

  newProcess("Hello World\n");
}

}  // namespace TestBindFrontInMemberFunction

namespace TestBindCreateNewCallableObject {
template <typename T>
void print(T t, const std::string& s) {
  std::cout << "Input arguments are: " << t << ", " << s << "\n";
}

void test() {
  int        i = 5;
  const auto f = std::bind(&print<int>, std::placeholders::_2,
                           std::placeholders::_1);  // input arguments type changed to (std::string, int)
  f("hello", i);                                    // 5 hello
  i = 6;
  f("world", i);  // 6 world

  i = 7;
  std::function<void(const std::string&, int)> f2(f);  // a new callable object
  f2("test", i);                                       // 7 test
}

}  // namespace TestBindCreateNewCallableObject

void test() {
  TestBindFrontInMemberFunction::test();
  TestBindCreateNewCallableObject::test();
}
}  // namespace TestSTDBindBasic

namespace TestSTDTransform {
// std::transform applies the given function to the elements of the given input range(s),
// and stores the result in an output range starting from d_first.

void test() {
  std::vector<int> vec{1, 2, 3, 4, 5, 6};
  std::vector<int> dst(vec.size());

  std::transform(vec.cbegin(), vec.cend(), dst.begin(), [](int x) { return x * x; });
  for (int i = 0; i < vec.size(); ++i) {
    myLog(vec[i]);
  }
}

}  // namespace TestSTDTransform

namespace TestSTDForEach {
// Applies the given function object f to the result of dereferencing every iterator in the range [first, last).
// If f returns a result, the result is ignored.
void test() {
  std::vector<int> vec{1, 2, 3, 4, 5, 6};
  std::for_each(vec.begin(), vec.end(), [](int& x) { x = x * 2; });
  for (int i = 0; i < vec.size(); ++i) {
    myLog(vec[i]);
  }
  splitLine();
  std::vector<int> vec1{10, 20, 30, 40, 50, 60};
  std::for_each(vec1.begin(), vec1.end(), [](int x) { x = x * 2; });
  for (int i = 0; i < vec1.size(); ++i) {
    myLog(vec1[i]);
  }
}

}  // namespace TestSTDForEach

namespace TestStdRanges {
void testStdViewsFilter() {
  std::vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto             even_numbers = numbers | std::views::filter([](int n) { return n % 2 == 0; });
  for (int n : even_numbers) {
    std::cout << n << " ";
  }
  std::cout << "\n";  // 2 4 6 8 10

  auto even_numbers2 = std::views::filter(numbers, [](int n) { return n % 2 == 0; });
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
  std::vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto             even_numbers = numbers | std::views::filter([](int& n) { return n % 2 == 0; });
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
  std::vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto             square_numbers  = numbers | std::views::transform([](int n) { return n * n; });
  auto             square_numbers2 = std::views::transform(numbers, [](int n) { return n * n; });

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
  std::vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto             res =
    numbers | std::views::filter([](int n) { return n % 2 == 1; }) | std::views::transform([](int n) { return n * n; });
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
  std::vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::ranges::for_each(numbers, [](int n) { n *= n; });

  for (int n : numbers) {
    std::cout << n << " ";
  }
  std::cout << "\n";  // 1 2 3 4 5 6 7 8 9 10

  std::ranges::for_each(numbers, [](int& n) { n *= n; });

  for (int n : numbers) {
    std::cout << n << " ";
  }
  std::cout << "\n";  // 1 4 9 16 25 36 49 64 81 100
}

void testStdRangesCount() {
  std::vector<int> numbers{1, 2, 3, 4, 5, 3, 7, 8, 3, 10};
  const auto       res = std::ranges::count(numbers, 3);
  std::cout << res << std::endl;  // 3
}

void testStdRangesCountIf() {
  std::vector<int> numbers{1, 2, 3, 4, 5, 3, 7, 8, 3, 10};
  const auto       res = std::ranges::count_if(numbers, [](int n) { return n >= 4; });
  std::cout << res << std::endl;  // 5
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

}  // namespace TestStdRanges

namespace TestSTDDistance {
void test() {
  std::vector<int> vec{3, 1, 9, 8};
  myLog(std::distance(vec.cbegin(), vec.cend()));  // 4
  myLog(std::distance(vec.cend(), vec.cbegin()));  // -4
}

}  // namespace TestSTDDistance

namespace TestSTDFindIf {
void testSTDFind() {
  std::vector<int> vec{3, 1, 9, 8};
  if (std::find(vec.cbegin(), vec.cend(), 9) != vec.cend()) {
    myLog("Found value.\n");
  } else {
    myLog("Vec does not contain 9");
  }
}

void testSTDFindIf() {
  std::vector<int> vec{3, 1, 9, 8};
  if (std::find_if(vec.cbegin(), vec.cend(), [](auto& val) { return val == 9; }) != vec.cend()) {
    myLog("Found value.\n");
  } else {
    myLog("Vec does not contain 9");
  }
}

void test() {
  testSTDFind();
  splitLine();
  testSTDFindIf();
}

}  // namespace TestSTDFindIf

namespace TestStdStringView {

void testStringViewBasic() {
  const char*      str = "this is a string";
  std::string_view sv1(str);
  myLog(sv1);
  splitLine();
  std::string_view sv2(str, 6);
  myLog(sv2);
}
}  // namespace TestStdStringView

namespace TestStdRefBasic {

void foo(int& n1, int& n2, const int& n3) {
  // std::cout << "In foo: n1: " << n1 << ", n2: " << n2 << ", n3: " << n3 << "\n";
  ++n1;
  ++n2;
  // ++n3  // compile error
}

void test() {
  int                   a1 = 1, a2 = 2, a3 = 3;
  std::function<void()> new_foo = std::bind(foo, a1, std::ref(a2), std::cref(a3));
  a1                            = 10;
  a2                            = 20;
  a3                            = 30;
  std::cout << "Before calling: a1: " << a1 << ", a2: " << a2 << ", a3: " << a3 << "\n";  // 10 20 30
  new_foo();
  std::cout << "After calling: a1: " << a1 << ", a2: " << a2 << ", a3: " << a3 << "\n";  // 10 21 30
}

}  // namespace TestStdRefBasic

namespace TestStdIota {
// include <numberic>
// Fills the range [first, last) with sequentially increasing values,
// starting with value and repetitively evaluating ++value.
void test() {
  std::vector<int> vec(10);
  std::iota(vec.begin(), vec.end(), 10);
  std::for_each(vec.cbegin(), vec.cend(), [](int x) { myLog(std::format(" {}", x)); });  // 10 11 12 13 14 15 16 ... 19
}

}  // namespace TestStdIota

namespace TestStdExecution {

// include <execution>
#ifdef PARALLEL
#include <execution>
namespace execution = std::execution;
#else
enum class execution { seq, unseq, par_unseq, par };
#endif

void measure([[maybe_unused]] auto policy, std::vector<std::uint64_t> v) {
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
  std::mt19937               gen{std::random_device{}()};
  std::ranges::generate(v, gen);

  measure(execution::seq, v);
  measure(execution::unseq, v);
  measure(execution::par_unseq, v);  // no parallel: 78ms, with parallel: 32ms
  measure(execution::par, v);        // no parallel: 81ms, with parallel: 37ms
}
}  // namespace TestStdExecution

namespace TestStdErase {
void test() {
  std::vector<int> list{1, 2, 3, 2, 4};
  std::erase(list, 2);  // 1, 3, 4
  [[maybe_unused]] int a = 1;
}

}  // namespace TestStdErase

namespace TestStdRemove {
/*
std::remove doesn't actually remove the elements, it moves forward all elements not equal to the given value and returns
a new end iterator that points to the new end after the "remove operation". It doesn't change the size of the container,
it just rearranges the elements.

erase will actually remove the element and resize the container.
erase takes two arguments indicating the range to be removed: from a start iterator to an end iterator (excluding the
end iterator).

*/
void test() {
  std::vector<int> list{1, 2, 3, 2, 4};
  list.erase(std::remove(list.begin(), list.end(), 3), list.end());  // 1, 2, 2, 4

  [[maybe_unused]] int a = 1;
}

}  // namespace TestStdRemove

namespace TestStdMinElement {
/*
        returns the smallest element in a range

*/
std::vector<int> v{3, 1, -4, 1, 5, 9};

void test() {
  const auto it = std::min_element(v.begin(), v.end());
  std::cout << "Value of smallest element: " << *it << " , index: " << std::distance(it, v.begin())
            << " , index in reverse order: " << std::distance(v.begin(), it);
  // -4, -2, 2
}

}  // namespace TestStdMinElement

namespace TestStdMaxElement {
/*
        returns the largest element in a range
*/
std::vector<int> v{3, 1, -4, 1, 5, 9};

void test() {
  const auto it = std::max_element(v.begin(), v.end());
  std::cout << "Value of smallest element: " << *it << " , index: " << std::distance(it, v.begin())
            << " , index in reverse order: " << std::distance(v.begin(), it);
  // 9, -5, 5
}

}  // namespace TestStdMaxElement

namespace TestStdExchange {
/*
        template< class T, class U = T >
        T exchange( T& obj, U&& new_value );
        Replaces the value of obj with new_value and returns the old value of obj.
*/
namespace UseCase1 {
// Resetting the value of a variable
void resetValue(int& value) {
  const int oldValue = std::exchange(value, 0);
  std::cout << "Old value: " << oldValue << std::endl;  // 42
}

void test() {
  int num = 42;
  resetValue(num);
  std::cout << "new value: " << num << std::endl;  // 0
}

}  // namespace UseCase1

namespace UseCase2 {
// simplify swaping operation
void test() {
  int x = 5, y = 10;
  std::cout << "x: " << x << ", y: " << y << std::endl;  // 5 10
  y = std::exchange(x, y);
  std::cout << "x: " << x << ", y: " << y << std::endl;  // 10 5
}

}  // namespace UseCase2

namespace UseCase3 {
// State transitions using std::exchange
enum class State {
  Idle,
  Running,
  Stopped

};

class Machine {
  public:
  State changeState(State sta) { return std::exchange(_curState, sta); }

  State getState() { return _curState; }

  private:
  State _curState = State::Idle;
};

void test() {
  Machine machine;
  std::cout << "Old state: " << static_cast<int>(machine.changeState(State::Running)) << std::endl;  // 0 (Idle)
  std::cout << "New state: " << static_cast<int>(machine.getState()) << std::endl;                   // 1 (Running)
}
}  // namespace UseCase3

namespace UseCase4 {
// Resource management and movement operations
class Resource {
  public:
  Resource(int value) : _data(new int(value)) {}

  // copy constructor
  Resource(const Resource& other) : _data(new int(*other._data)) {}

  // move constructor
  Resource(Resource&& other) noexcept : _data(std::exchange(other._data, nullptr)) {}

  // move assignment operator
  Resource& operator=(Resource&& other) noexcept {
    if (this != &other) {
      delete _data;
      _data = std::exchange(other._data, nullptr);
    }
    return *this;
  }

  // copy assignment operator
  Resource& operator=(Resource& other) noexcept {
    if (this != &other) {
      delete _data;
      _data = new int(*other._data);
    }
    return *this;
  }

  ~Resource() { delete _data; }

  int getValue() { return _data ? *_data : 0; }

  private:
  int* _data;
};

void test() {
  Resource res1(42);
  Resource res2(std::move(res1));  // call move constructor
  // Resource res2 = std::move(res2); // call move constructor
  std::cout << "res1 value: " << res1.getValue() << std::endl;  // 0
  std::cout << "res2 value: " << res2.getValue() << std::endl;  // 42

  Resource res3(100);
  res3 = std::move(res2);                                       // call move assignment operator
  std::cout << "res2 value: " << res2.getValue() << std::endl;  // 0
  std::cout << "res3 value: " << res3.getValue() << std::endl;  // 42
}

}  // namespace UseCase4
void test() {
  UseCase1::test();
  UseCase2::test();
  UseCase3::test();
  UseCase4::test();
}

}  // namespace TestStdExchange

namespace TestStdAny {
/*

std::any is a standard library type introduced by C++17 that provides a type-safe way to store and manipulate values of
any type. It can store any copy-constructed assignable type, and the stored values can be checked and extracted at
runtime.

Summary:
std::any provides a flexible and type-safe way to store and manipulate values of any type, making it useful in
situations where you need to store different types of data and are unsure of the exact type.These dynamically typed data
can be managed and used efficiently by extracting values using std::any_cast in conjunction with type checking and
exception handling.
*/
void testAnyBasic() {
  std::any a = 10;
  if (a.has_value()) {
    std::cout << "variable contains a value\n";  // yes
  }
  std::cout << "value of a : " << std::any_cast<int>(a) << "\n";  // 10

  try {
    std::cout << "value of a : " << std::any_cast<std::string>(a) << "\n";  // no

  } catch (const std::bad_cast& e) {
    std::cout << "bad cast exception\n";  // yes
  }

  a.reset();
  if (!a.has_value()) {
    std::cout << "variable does not contain a value\n";  // yes
  }

  a = std::string("Hello world");
  std::cout << "new value of a : " << std::any_cast<std::string>(a) << "\n";
}

void testAnyInPointer() {
  std::any a = 10;
  if (auto aPtr = std::any_cast<int*>(&a)) {
    std::cout << "value: " << *aPtr << "\n";  // no
  }

  if (auto aPtr = std::any_cast<int>(&a)) {
    std::cout << "value: " << *aPtr << "\n";  // yes
  }

  int ba = 1;
}

void test() {
  testAnyBasic();
  testAnyInPointer();
}

}  // namespace TestStdAny

namespace TestStdInvoke {
/*
std::invoke is a function template introduced in C++17 for generic invocation of callable objects. std::invoke can call
a variety of callable objects such as function pointers, member function pointers, function objects, and lambda
expressions.It can call a variety of callable objects such as function pointers, member function pointers, function
objects, and lambda expressions. std::invoke provides a unified way to work with these different types of callable
objects, simplifying the writing and calling of code.
*/
void print(int i, const std::string& s) {
  std::cout << "normal function called, i: " << i << ", s: " << s << "\n";
}

class Printer {
  public:
  void print(int i, const std::string& s) {
    std::cout << "class print function called, i: " << i << ", s: " << s << "\n";
  }
};

auto lambdaFunc = [](int i, const std::string& s) {
  std::cout << "lambda expression called, i: " << i << ", s: " << s << "\n";
};

void testInvokeBasic() {
  // normal functions
  std::invoke(print, 1, "hello");
  // member function
  Printer p;
  std::invoke(&Printer::print, p, 2, "world");
  // lambda expression
  std::invoke(lambdaFunc, 3, "test");
}
}  // namespace TestStdInvoke

namespace TestHandleDifferentContainersWithConstexprIf {
template <typename T>
void printContainer(const T& t) {
  if constexpr (std::is_same<T, std::vector<typename T::value_type>>::value) {
    std::cout << "T is a vector\n";
  } else if constexpr (std::is_same<T, std::list<typename T::value_type>>::value) {
    std::cout << "T is a list\n";
  } else if constexpr (std::is_same<T, std::queue<typename T::value_type>>::value) {
    std::cout << "T is a queue\n";
  }

  else {
    std::cout << "Unknow type\n";
  }
}

void test() {
  std::vector<int>  vec{1, 2, 3};
  std::list<double> list{1.0, 2.0, 3.0};
  std::queue<float> queue;
  queue.push(9.0f);
  printContainer(vec);
  printContainer(list);
  printContainer(queue);
}

}  // namespace TestHandleDifferentContainersWithConstexprIf

namespace TestStdQuoted {
/*
C++14. This feature is part of the C++ Standard Library and is used to handle quoted strings in input and output
streams, making it easier to work with strings that contain spaces or special characters
*/

void testWritingQuotedStrings() {
  /*
  Output (Writing Quoted Strings): When outputting a string, std::quoted adds quotes around the string and escapes any
  embedded quotes.
  */
  const std::string s = "Hello, \"world\"!";

  std::cout << s << std::endl;  // Hello, "world"!
  std::cout << "*******************************\n";
  std::cout << std::quoted(s) << std::endl;  // "Hello, \"world\"!"
  int a = 1;
}

void testReadingQuotedStrings() {
  /*
  Input (Reading Quoted Strings): When reading a string, std::quoted strips the surrounding quotes and unescapes any
  escaped characters.
  */
  std::istringstream input1("Hello\"world\"!");
  std::istringstream input2("Hello\"world\"!");
  std::string        s1, s2;
  input1 >> s1;
  input2 >> std::quoted(s2);
  std::cout << s1 << "\n";  // Hello"world"!
  std::cout << "********************\n";
  std::cout << s2 << "\n";  // Hello"world"!
  int a = 1;
}

void testAdvancedUsage() {
  /*
  C++ 14 std::quoted can also take additional parameters for customizing the quoting and escaping behavior:

  Delimiter: The character used for quoting.
  Escape Character: The character used for escaping embedded quotes and the escape character itself.
  */

  std::string s = "Hello, 'world'!";
  std::cout << std::quoted(s, '\'') << std::endl;  // Outputs: 'Hello, \'world\'!'

  // In this string, double backslashes \\ are used to indicate escape characters.The first backslash in each backslash
  // pair is used to escape the second backslash, making the actual backslash passed to the The string for
  // std::istringstream is 'Hello, \'world\'!' The single quote \' here indicates an escaped single quote character.
  std::istringstream input("'Hello, \\'world\\'!'");
  std::string        t;
  input >> std::quoted(t, '\'');
  std::cout << t << std::endl;  // Outputs: Hello, 'world'!
}

void test() {
  testWritingQuotedStrings();
  testReadingQuotedStrings();
  testAdvancedUsage();
}

}  // namespace TestStdQuoted

namespace TestStdRegex {
/*
introduced in C++11, provides powerful tools for working with regular expressions. Regular expressions (regex) are
patterns used to match character combinations in strings, making them useful for tasks such as searching, matching, and
replacing text.
*/

void testBasic() {
  const std::string str = "hello world";
  std::regex        pattern{"world"};
  if (std::regex_search(str, pattern)) {
    std::cout << "Pattern found\n";  // yes
  }
}

void testStdRegexMatch() {
  /*
  Summary
std::regex_match requires the entire string to match the pattern.
std::regex_search finds a match for any part of the string.
  */
  const std::string str1 = "hello world cumbersome";
  const std::string str2 = "hello w2orld cumbersome";
  const std::string str3 = "hello world";
  const std::regex  pattern{"^hello world$"};  // For std::regex_match to return true, the entire string must match the
                                               // pattern from start (^) to end ($).
  const auto res1 = std::regex_match(str1, pattern);   // false
  const auto res2 = std::regex_match(str2, pattern);   // false
  const auto res3 = std::regex_match(str3, pattern);   // true
  const auto res5 = std::regex_search(str1, pattern);  // false

  const std::regex pattern2{"hello world"};  // Pattern: "^hello world" removes the $ at the end, so it only checks if
                                             // the string starts with "hello world".
  const auto res4 = std::regex_search(str1, pattern2);  // true
  const auto res6 = std::regex_search(str2, pattern2);  // false
  int        a    = 1;
}

void testCapturingGroups() {
  /*
   \w: This matches any word character (equivalent to [a-zA-Z0-9_]).
     +: This quantifier matches one or more of the preceding token, so \w+ matches one or more word characters.
    The parentheses () create a capture group, which allows you to extract the matched portion of the string.

    */
  const std::string s{"my email is surname.zuo@gmail.com"};
  std::regex        pattern{R"(\w+\.\w+@\w+\.\w+)"};
  std::smatch       matches;
  const auto        res   = std::regex_search(s, matches, pattern);
  const auto        email = matches[0];  // surname.zuo@gmail.com
  int               a     = 1;
}

void testReplacingSubstrings() {
  const std::string s{"hello world"};
  std::regex        pattern{"world"};
  const std::string replacement{"C++"};

  const auto res = std::regex_replace(s, pattern, replacement);
  std::cout << res << std::endl; // hello C++

}

void test() {
  testBasic();
  testStdRegexMatch();
  testCapturingGroups();
  testReplacingSubstrings();
}

}  // namespace TestStdRegex

void test() {
  TestStdRegex::test();
  // TestStdQuoted::test();
  //  TestHandleDifferentContainersWithConstexprIf::test();

  // TestStdInvoke::testInvokeBasic();

  // TestStdAny::test();
  //  TestStdExchange::test();

  // TestStdMinElement::test();

  // TestStdErase::test();
  // TestStdRemove::test();

  // TestStdExecution::testExecution();
  // TestStdIota::test();
  // TestStdRefBasic::test();
  // TestSTDBindBasic::test();
  // TestStdStringView::testStringViewBasic();
  // TestSTDDistance::test();
  // TestSTDFindIf::test();
  // TestSTDForEach::test();
  // TestStdRanges::test();
  // TestSTDForEach::test();
  // TestSTDTransform::test();
  // TestSTDForward::TestSTDForwardWithinClass::test();
  // TestSTDForward::TestLRValueWithinClass::test();
  // TestSTDForward::testLRvalueBasic();
  // TestSTDFunctionBasic::test();
  // TestSTDBindBasic::testBind();
}