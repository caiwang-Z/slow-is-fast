#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <variant>
#include <vector>
#include "utility.h"

using UtilityNameSpace::myLog;
using UtilityNameSpace::splitLine;
using UtilityNameSpace::Splitter;

namespace TestPtrVariable {
int width = 10, height = 2;

void testAddressOfPointerVariable() {
  unsigned char* raw            = new unsigned char[width * height];
  auto           rawAdd         = &raw;  //
  auto           rawAddValue    = *rawAdd;
  auto           rawAddAdd      = &rawAdd;
  auto           rawAddAddValue = *rawAddAdd;
}

void fill(unsigned char* ptr, int val) {
  int count = width * height;
  for (int i = 0; i < count; ++i) {
    ptr[i] = val;
  }
}

void read(unsigned char* ptr) {
  std::cout << "Value:";
  for (int i = 0; i < width * height; ++i) {
    std::cout << static_cast<int>(ptr[i]) << ", ";
  }
  std::cout << std::endl;
}

void convertWithPassingValue(unsigned char* rawData) {
  unsigned char* newData = new unsigned char[width * height];
  fill(newData, 22);
  delete rawData;     // resources of object in memory address rawData are released.
  rawData = newData;  // rawData is only a copy of raw and it points the memory addresss of raw, after this line, it
                      // points to the memory address of newData
}

void convertWithPassingReference(unsigned char*& rawData) {
  unsigned char* newData = new unsigned char[width * height];
  fill(newData, 22);
  delete rawData;     // resources of object in memory address rawData are released.
  rawData = newData;  // rawData is only a reference of raw and it points the memory addresss of raw, after this line,
                      // both it and raw point to the same memory address of newData
}
void test1() {
  unsigned char* raw = new unsigned char[width * height];
  fill(raw, 11);  // 11 11 11 ...
  read(raw);
  convertWithPassingReference(raw);  // resources in raw are released, raw is invalid
  read(raw);                         // random number like 221, 221, ...
}

void test2() {
  unsigned char* raw = new unsigned char[width * height];
  fill(raw, 11);  // 11 11 11 ...
  read(raw);
  convertWithPassingValue(raw);  // raw points to a valid memory address of newData;
  read(raw);                     // 22 22 22 ...
}

void test() {
  test1();
  // test2();
  // testAddressOfPointerVariable();
}
}  // namespace TestPtrVariable

namespace TestBitShift {

void checkEndianess() {
  uint32_t       value = 0x12345678;  // one 32bit value
  unsigned char* ptr   = reinterpret_cast<unsigned char*>(&value);

  std::cout << "Memory layout: ";
  for (size_t i = 0; i < sizeof(value); ++i) {
    printf("%02X ", ptr[i]);
  }
  std::cout << std::endl;

  if (ptr[0] == 0x78) {
    std::cout << "Little-endian" << std::endl;
  } else if (ptr[0] == 0x12) {
    std::cout << "Big-endian" << std::endl;
  } else {
    std::cout << "Unknown endianness!" << std::endl;
  }
}

void processRawData(unsigned char* rawPtr, size_t pixelCount) {
  uint16_t* pixelPtr = reinterpret_cast<uint16_t*>(rawPtr);

  for (size_t i = 0; i < pixelCount; ++i) {
    uint16_t pixelValue = pixelPtr[i];

    uint16_t newPixelValue = pixelValue >> 6;

    pixelPtr[i] = newPixelValue;
  }
}

void readValue(unsigned char* rawPtr, size_t pixelCount) {
  uint16_t* pixelPtr = reinterpret_cast<uint16_t*>(rawPtr);

  std::cout << "Pixel value are: ";
  for (size_t i = 0; i < pixelCount; ++i) {
    uint16_t pixelValue = pixelPtr[i];

    std::cout << pixelValue << ", ";
  }
  std::cout << std::endl;
}

void test1() {
  // because win10 is little endian, so the first number would be 0x1234(hex) == 4660(decimal)
  unsigned char rawData[] = {0x34, 0x12, 0xCD, 0xAB, 0xEE, 0xFF, 0x23, 0x01};

  size_t pixelCount = sizeof(rawData) / 2;
  readValue(rawData, pixelCount);

  processRawData(rawData, pixelCount);

  readValue(rawData, pixelCount);
}

void test() {
  test1();
  // checkEndianess();
}

}  // namespace TestBitShift

namespace TestStopUsingSTDEndl {

/*
*
std::endl��Insert newline and flush
std::ends��Insert null character
std::flush��Flush stream buffer
*
1.What is Buffer?
Buffer is also known as cache, which is a part of memory space.That is to say,
a certain amount of storage space is reserved in the memory space which is used
to buffer input or output data, and this reserved space is called buffer.Buffers
are categorized into input buffers and output buffers depending on whether they
correspond to input or output devices.

2. Why do we need to introduce buffers?
For example, we take information from the disk, we first read out the data in
the buffer, the computer and then directly from the buffer to take the data,
such as buffer data to take the end of the disk and then go to read, so you can
reduce the number of times the disk read and write, coupled with the computer's
operation of the buffer is much faster than the operation of the disk, it is the
application of the buffer can greatly increase the speed of computer
operations.Another example, we use the printer to print documents, due to the
printer's printing speed is relatively slow, we first output the document to the
printer's corresponding buffer, the printer and then gradually print their own,
then our CPU can deal with other things. Now you basically understand it, the
buffer is a piece of memory, it is used between the input and output devices and
the CPU, used to cache data.It allows low-speed I/O devices and high-speed CPU
to work in harmony, avoiding low-speed I/O devices from occupying the CPU and
freeing the CPU to work efficiently.

3.Buffer Flush
Flushing a buffer is the process of taking the data in the buffer, performing
real I/O, and then emptying the buffer.As you can see from the example above,
the following situations trigger a buffer flush:
a. when the buffer is full;
b. at the end of the program;
c. by executing the flush statement;
d. by closing the file, i.e., by executing the close statement;
e. by executing the endl statement.

*/

void test() {
  std::ofstream outFile("test.txt");
  for (int n = 0; n < 4096; ++n) {
    outFile << 'A';
  }  // the data is still in buffer, not in the file. No I/O process.

  std::ofstream outFile1("test1.txt");
  for (int n = 0; n < 4096; ++n) {
    outFile1 << 'A';
  }
  outFile1 << std::endl;  // data in buffer has been writen into file.

  std::ofstream outFile2("test2.txt");
  for (int n = 0; n < 4096; ++n) {
    outFile2 << 'A';
  }
  outFile2.close();  // data in buffer has been writen into file.

  {
    std::ofstream outFile3("test3.txt");
    for (int n = 0; n < 4096; ++n) {
      outFile3 << 'A';
    }
    int a = 1;  // data in buffer has been writen into file.
  }

  int a = 1;
}

}  // namespace TestStopUsingSTDEndl

namespace TestNameDemangling {
/*
C++ Name Demangling is the process of restoring the "mangled" names of symbols
(e.g., function names, variable names, etc.) that are internally processed by
the compiler in C++ code to their original human-readable names.Since C++
supports function overloading and other advanced features, the compiler modifies
the names of symbols when generating the object code to uniquely identify each
symbol, a process called "name mangling".

However, such modified symbol names are very difficult for humans to understand and debug.The technique of "name
demangling" is needed to restore these symbols to their original readable names.The exact methods of mangling and
demangling may vary between platforms and compilers.Some commonly used tools and libraries, such as the c++filt tool
provided by GCC, can be used to perform this process.

Common Uses:
* Debugging: Debuggers and analysis tools utilize demangling to convert symbolic names into human-readable
    form to help developers locate problems.
* Logging and Bug Reporting: The use of demangling can provide more meaningful
information in the stack trace generated when a program crashes.
* Performance analysis: Performance analysis tools
demangling the collected symbol information so that developers can analyze performance bottlenecks.

Summary
C++ Name Demangling is the process of reducing the internal names generated by the compiler to handle C++ symbols to
their original, readable names.This is important for debugging, log analysis, and performance optimization.

*/

}  // namespace TestNameDemangling

namespace TestClangFormat {
/*
1. file name must be .clang-format. And put in the root directory of the project
2. enable clang format support in visual studio
3. Ctrl + K and Ctrl + D, apply code formating  for current file
*/

}

namespace TestFoldExpression {
namespace TestMonadicLeftFolding {
// Monadic left folding
// (... operation pack)

template <typename... Args>
auto sum(Args... args) {
  return (... + args);  // Sum the argument packet args from left to right.
}
}  // namespace TestMonadicLeftFolding

namespace TestMondadicRightFolding {
// Monadic right folding
// (pack operation ...)

template <typename... Args>
auto sum(Args... args) {
  return (args + ...);  // sum the argument packet args from right to left.
}

}  // namespace TestMondadicRightFolding

namespace TestBinaryLeftFolding {
// Binary left folding
// (initValue operation ... operation pack)
template <typename... Args>
auto sum(Args... args) {
  return (10 + ... + args);
}

}  // namespace TestBinaryLeftFolding

namespace TestBinaryRightFolding {
// Binary right folding:
// (pack operation ... operation initValue)

template <typename... Args>
auto sum(Args... args) {
  return (args + ... + 20);
}

}  // namespace TestBinaryRightFolding

void test() {
  const auto res1 = TestMonadicLeftFolding::sum(1, 2, 3, 4, 5);
  const auto res2 = TestMondadicRightFolding::sum(1, 2, 3, 4, 5);
  const auto res3 = TestBinaryLeftFolding::sum(1, 2, 3, 4, 5);
  const auto res4 = TestBinaryRightFolding::sum(1, 2, 3, 4, 5);
  int        a    = 1;
}

}  // namespace TestFoldExpression

namespace TestVariadicExpansionWrapUp {
/*
In C++, variadic templates allow a function to take an arbitrary number of arguments.Varadic expansion is the process of
expanding these parameters and processing them.Varadic Expansion Wrap-Up usually involves expanding the varadic template
parameters and performing some kind of processing.
*/
template <typename T>
void print(T t) {
  std::cout << t << "\n";
}

template <typename T, typename... Args>
void print(T t, Args... args) {
  std::cout << t << "\n";  // comment this line: try world; No comment: try 1 2.5 Hello world
  print(args...);
}

/*
In some cases, you may want to process all the variables in a single operation, rather than one by one.In this case, you
can use the "wrapping" technique of variant expansion, such as initializing lists or collapsing expressions (a feature
introduced in C++17) to handle all the arguments at once.

*/
template <typename... Args>
void print_all(Args... args) {
  //((std::cout << args << std::endl), ...);
  //(..., (std::cout << args << "\n") );
  ((std::cout << args << "\n"), ...);
}

void test() {
  print("try");
  print(1, 2.5, "Hello", "world");
  print_all("new", "world", 4, 8, 90);
}

}  // namespace TestVariadicExpansionWrapUp

namespace TestAssignmentInIfExpression {
/*
The assignment operation itself is an expression, and it results in the left operand of the assigned value.

This behavior allows assignment operations to be used as part of conditional expressions.
*/
void test() {
  const std::map<std::string, int> myMap{{"one", 1}, {"two", 2}};
  if (auto it = myMap.find("two"); it != myMap.end()) {
    std::cout << "Found element\n";

  } else {
    std::cout << "Finding element failed\n";
  }
}
}  // namespace TestAssignmentInIfExpression

namespace TestIfAndSwitchInitStatements {
/*
C++17's if and switch Init Statements
This allows you to define and initialize variables in the same scope and then immediately use them for conditional
judgments.

This approach avoids the need to define value variables outside of an if statement, and makes the code more compact and
clearer
*/
void testIfInitStatement() {
  const std::vector<int> vec{1, 2, 3, 4};
  if (auto itr = std::find(vec.cbegin(), vec.cend(), 3); itr != vec.end()) {
    std::cout << "Found value: " << *itr << "\n";  // 3
  }  // cons: itr is not visible outside or inside this function. I do not allow it.
}

void testSwitchInitStatement() {
  const std::string var{"example"};
  switch (auto len = var.length(); len) {
    case 0:
      // do something
      break;
    case 1:
      // do something
      break;
    case 7:
      std::cout << "length is: " << len << std::endl;  // 7
      break;
  }
}

void test() {
  testIfInitStatement();
  testSwitchInitStatement();
}

}  // namespace TestIfAndSwitchInitStatements

namespace TestNestedNamespace {
/*
Before C++17, to define multiple nested namespaces, you had to define them layer by layer:
*/
namespace A {
namespace B {
namespace C {
void foo() {}
}  // namespace C
}  // namespace B
}  // namespace A

/*
C++17 introduced the nested namespace syntax, which allows you to define multiple nested namespaces on a single line:
This makes namespace definitions more concise and clear, especially if the hierarchy is deeper

*/

namespace E::F::G {
void foo() {}
}  // namespace E::F::G

void test() {
  A::B::C::foo();
  E::F::G::foo();
}

}  // namespace TestNestedNamespace

namespace TestHasInclude {
/*
C++17 introduces a preprocessor directive, __has_include, which allows checking for the existence of a header file
during compilation.This is useful for conditional compilation and cross-platform programming, where different code paths
can be chosen depending on the availability of header files.

Example 1:

#if __has_include(<header>)
    // Code if header is available
#else
    // Code if header is not available
#endif

Example 2:
#include <iostream>

#if __has_include(<filesystem>)
    #include <filesystem>
    namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
    #error "No filesystem support"
#endif

int main() {
    fs::path p = "/path/to/somewhere";
    std::cout << "Path: " << p << std::endl;
    return 0;
}

Summary:
C++17's __has_include is a powerful tool that allows programmers to check the availability of header files
during compilation, leading to conditional compilation and better cross-platform compatibility.By using __has_include
flexibly, it is possible to write more robust and adaptable code.

*/

}

namespace TestStructuredBindings {
/*
Summary
C++17's structured bindings provide a concise and intuitive way to deconstruct complex data structures, making
code easier to read and maintain.By using structured bindings wisely, you can improve code readability and development
efficiency in many scenarios.
*/

void testDeconstructingTuples() {
  std::tuple<int, std::string, float> tp{10, "lee", 1.89f};
  auto [age, name, height] = tp;
  std::cout << "Name: " << name << ", age: " << age << ", height: " << height << "\n";
}

namespace testDeconstructingStruct {
struct Person {
  std::string name;
  int         age;
  float       height;
};
void test() {
  Person ps{"Jack", 63, 1.90f};
  auto [name, age, height] = ps;
  std::cout << "Name: " << name << ", age: " << age << ", height: " << height << "\n";
}

}  // namespace testDeconstructingStruct

void testDeconstructingArray() {
  int arr[]{11, 22, 33};
  auto [a, b, c] = arr;
  std::cout << "a: " << a << ", b: " << b << ", c: " << c << "\n";
}

namespace TestDeconstructingFuncReturnValues {
std::pair<int, std::string> foo() {
  return {99, "david"};
}

void test() {
  auto [age, name] = foo();
  std::cout << "Name: " << name << ", age: " << age << "\n";
}
}  // namespace TestDeconstructingFuncReturnValues

void testDeconstructingSTDMap() {
  std::map<int, std::string> map{{1, "jack"}, {2, "manda"}, {45, "kk"}};
  for (const auto& [id, name] : map) {
    std::cout << "Key: " << id << ", name: " << name << "\n";
  }
}

void test() {
  testDeconstructingTuples();
  testDeconstructingStruct::test();
  testDeconstructingArray();
  TestDeconstructingFuncReturnValues::test();
  testDeconstructingSTDMap();
}

}  // namespace TestStructuredBindings

namespace TestAggregateInitialization {
/*
C++ 17 Definition of an Aggregate:
A type is an aggregate if it satisfies all of the following conditions: it is an array type
or class (usually a structure) and has no user-supplied constructor.It has no private or protected non-static data
members.It has no base classes.It has no virtual member functions.

Initialization Syntax:
An aggregation can be
initialized using an initialization list surrounded by curly braces.The following are some examples to illustrate
aggregate initialization:
*/
namespace TestInitlizationStruct {
struct Point {
  int x;
  int y;
};

void test() {
  Point p1{1, 2};
  Point p2{3, 4};
}
}  // namespace TestInitlizationStruct

namespace TestNestedAggregation {
struct Rectange {
  TestInitlizationStruct::Point topLeft;
  TestInitlizationStruct::Point bottomRight;
};

void test() {
  Rectange rect{{1, 2}, {3, 4}};
}
}  // namespace TestNestedAggregation

namespace TestAggregationWithDefaultMemberInitializer {
struct Point {
  float x = 1.1f;
  float y = 2.2f;
};

void test() {
  Point p1{8.9, 4.5};
  Point p2;
}

}  // namespace TestAggregationWithDefaultMemberInitializer

}  // namespace TestAggregateInitialization

namespace TestFloatingLiteralsDefinition {
/*
Summary The f or F suffix is used to specify a floating-point literal as a float.By default, float literals are treated
as double.Using the f suffix saves memory, improves performance, and avoids unnecessary implicit conversions.
*/

void test() {
  float f1 = 3.14;   // Implicitly a double, then converted to float
  float f2 = 3.14f;  // Explicitly a float

  double      d1  = 3.14;   // Default to double
  long double ld1 = 3.14L;  // Explicitly a long double

  std::cout << "size of f1: " << sizeof(f1) << " bytes.\n";  // 4
  std::cout << "size of f2: " << sizeof(f2) << " bytes.\n";  // 4
  std::cout << "size of d1: " << sizeof(d1) << " bytes.\n";  // 8
  std::cout << "size of ld1: " << sizeof(ld1)
            << " bytes.\n";  // 16 (depends on hardware architecture, platfform, win10 msvc 8 bytes)
}

}  // namespace TestFloatingLiteralsDefinition

namespace {
/*
Benefits
Code Clarity: By explicitly indicating intentional fall-throughs, the [[fallthrough]] attribute makes the code easier to
read and understand. Error Prevention: It helps prevent bugs that could arise from unintended fall-throughs by making
the programmer's intent explicit. Suppressing Warnings: It suppresses compiler warnings about fall-through, which can
otherwise clutter the output and obscure actual issues.

Compiler Warnings
Prior to C++17, compilers often issued warnings for fall-through cases in switch statements to avoid potential bugs. The
[[fallthrough]] attribute can be used to suppress these warnings by explicitly stating the programmer's intention.

*/
void example(int n) {
  switch (n) {
    case 1:
      std::cout << "Case 1" << std::endl;
      [[fallthrough]];  // Indicate intentional fall-through
    case 2:
      std::cout << "Case 2" << std::endl;
      break;
    case 3:
      std::cout << "Case 3" << std::endl;
      break;
    default:
      std::cout << "Default case" << std::endl;
      break;
  }
}

}  // namespace

namespace TestStartUsingDefaultMemberInitializer {
/*
Key Points
What are Default Member Initializers?
Default member initializers allow you to assign default values to class members at the point of their declaration within
the class definition. This eliminates the need to repeatedly initialize these members in multiple constructors.

Syntax
The syntax for default member initializers is straightforward. You assign a default value directly in the member's
declaration:

Benefits
Simplified Constructors:
Default member initializers reduce the need to explicitly initialize members in every constructor, making the
constructors cleaner and easier to read.

Code Reuse and Maintenance:
If a member's default value needs to change, you only need to update it in one place rather than in every constructor.

Improved Readability:
It is easier to see the default values of members directly in the class definition, improving code readability.

Consistency:
Ensures that all constructors, including the default constructor, initialize members consistently.
*/
namespace WithoutMemberInitializer {
class Person {
  public:
  Person(const std::string& name, int age, int height) : _name(name), _age(age), _height(height) {}
  void print() const { std::cout << "Name: " << _name << ", age: " << _age << ", hegiht: " << _height << " cm \n"; }

  private:
  std::string _name;
  int         _age;
  int         _height;
};

void test() {
  // Person p1{}; // compile error
  Person p2{"lee", 12, 189};
  p2.print();
}
}  // namespace WithoutMemberInitializer

namespace WithMemberInitializer {
class Person {
  public:
  Person() = default;
  Person(const std::string& name, int age, int height) : _name(name), _age(age), _height(height) {}
  void print() const { std::cout << "Name: " << _name << ", age: " << _age << ", hegiht: " << _height << " cm \n"; }

  private:
  std::string _name   = "jacky";
  int         _age    = 78;
  int         _height = 178;
};

void test() {
  Person p1{};  // no compile error
  p1.print();
}
}  // namespace WithMemberInitializer
void test() {
  WithoutMemberInitializer::test();
  WithMemberInitializer::test();
}
}  // namespace TestStartUsingDefaultMemberInitializer

namespace TestConstexprLambdaSupport {
/*
Benefits of constexpr Lambdas C++17
Performance: By moving computations to compile time, constexpr lambdas can help improve runtime performance.
Safety: Compile-time evaluation ensures that the results are correct before the program runs, catching potential errors
early. Readability: constexpr lambdas can make code involving compile-time computations more readable and maintainable.
*/

/*
first constexpr (constexpr auto add = ...): This tells the compiler that add is a constexpr variable, meaning that add
is evaluated and initialized at compile time.add holds a constexpr lambda expression that can be used in constant
expressions.

The second constexpr ([](int a, int b) constexpr {...}): This tells the compiler that the lambda expression
itself is a constexpr, meaning that as long as the argument passed to the lambda is a constant expression, then the
result of the lambda call is also a constant expression.This allows us to use add in constexpr int result = add(3, 4);
because the result of a call to add can be computed at compile time.
*/

constexpr auto add = [](int a, int b) constexpr { return a + b; };

void test() {
  constexpr int result = add(3, 4);  // get result during compiling
}

}  // namespace TestConstexprLambdaSupport

namespace TestVariadicUsing {
/*
the variadic using declaration introduced in C++17. This feature simplifies the process of inheriting multiple
constructors from base classes in template metaprogramming by allowing you to inherit constructors from multiple base
classes more concisely.
*/

class Base1 {
  public:
  Base1(int x) { std::cout << "Base1 constructor called\n"; }
};

class Base2 {
  public:
  Base2(float x) { std::cout << "Base2 constructor called.\n"; }
};

class Base3 {
  public:
  Base3(const std::string& name) { std::cout << "Base3 constructor called.\n"; }
};

template <typename... Bases>
class Derived3 : public Bases... {
  public:
  using Bases::Bases...;
};

void test() {
  Derived3<Base2> del4(6.7f);
  Derived3<Base1> del5(4);
  Derived3<Base3> del6("jacky");
  int             a = 1;
}

}  // namespace TestVariadicUsing

namespace TestExternCandDLLExportImport {
/*
#ifndef MAVISION_COMMON_DEFINITIONS_H
#define MAVISION_COMMON_DEFINITIONS_H

// When building dynamic libraries, symbols need to be exported so that they are visible to the application that uses
the dynamic library.When using a dynamic library, symbols need to be imported so that the application knows how to call
these exported functions.
#ifdef _WIN32
#ifdef MAVISION_BUILD_DLL
#define MAVISION_DLL_EXPORT __declspec(dllexport)
#else
#define MAVISION_DLL_EXPORT __declspec(dllimport)
#endif
// On non-Windows platforms, all symbols are visible by default, which can lead to naming conflicts and larger library
files.Using __attribute__((visibility("default"))) allows you to explicitly control which symbols are visible, which
reduces naming conflicts, improves loading speed and reduces the size of library files.
#else
#ifdef MAVISION_BUILD_DLL
#define MAVISION_DLL_EXPORT __attribute__((visibility("default")))
#else
#define MAVISION_DLL_EXPORT
#endif
#endif

// Use the extern "C" block to ensure that the C++ compiler does not name-modify function names so that C++ code can
call C functions. #ifdef __cplusplus extern "C" { #endif

///
enum maErr {
  maErrOK,
  maErrCamera,
  maErrTimeout,
  maErrAbort,
  maErrFunctionDeactivated,
  maErrFileIO,
  maErrImageProcessing,
  maErrUnknown

};

MAVISION_DLL_EXPORT maErr vscStart();

MAVISION_DLL_EXPORT maErr vscStop();

#ifdef __cplusplus
}
#endif

#endif


*/

}

namespace TestGotoStatement {
/*
Common Uses of goto:
Despite its bad reputation, goto can be useful in certain scenarios, such as:
Error Handling and Cleanup: In low-level programming or resource management, goto can simplify error handling and
cleanup code by providing a single exit point.

Breaking Out of Multiple Loops: goto can be used to break out of nested loops when other control flow mechanisms are
less clear or more cumbersome.

Pitfalls of goto:
While goto can be useful, it also has significant pitfalls:

Spaghetti Code: Overuse of goto can lead to tangled, hard-to-follow code, commonly referred to as "spaghetti code."
Maintenance Challenges: Code that relies heavily on goto can be difficult to maintain and debug.
Readability: goto can obscure the logical flow of the program, making it harder for others (or yourself) to understand
the code later.

*/

namespace TestErrorHandlingAndCleanup {
int openSource() {
  std::cout << "Resource opend.\n";
  return 0;  // return 0 on success
}

void closeResource() {
  std::cout << "Resource closing...\n";
  std::cout << "Resource closed.\n";
}

int test() {
  int error = 0;
  error     = openSource();
  if (error) {
    goto cleanup;  // no
  }

  error = 1;
  if (error) {
    goto cleanup;  // yes
  }

cleanup:
  closeResource();
  return error;  // 1
}
}  // namespace TestErrorHandlingAndCleanup

void testBreakingOutOfNestedLoops() {
  bool found = false;
  for (int i = 0; i <= 10; ++i) {
    for (int j = 0; j < 100; ++j) {
      if (i * j == 42) {
        found = true;
        goto endLoops;
      }
    }
  }

endLoops:
  if (found) {
    std::cout << "Found\n";

  } else {
    std::cout << "Not found\n";
  }
}

void test() {
  TestErrorHandlingAndCleanup::test();
  testBreakingOutOfNestedLoops();
}

}  // namespace TestGotoStatement

namespace TestRawStringLiterals {
/*
In C++, R"()" is a syntax called "Raw String Literals".This syntax allows you to define strings without using
backslashes (\) to escape special characters.Using raw string literals makes it easier to write strings that contain
many special characters, such as regular expressions, file paths, or multi-line text.
*/

void testBasic() {
  /*
  R "delimiter(string content) delimiter"
  R indicates that this is a raw string.
  delimiter is an optional delimiter, which can be any set of characters, used to define the beginning and end of the
  string. Inside () is the actual string content.
  */
  const auto res1{"this is a normal line"};
  const auto res11{R"(this is a normal line)"};
  std::cout << res1 << "\n";   // this is a normal line
  std::cout << res11 << "\n";  // this is a normal line

  const auto res2{"this is a line with \"quotes\" and \(bracket\)"};
  // const auto res4{"this is a line with "quotes" and bracket"};  // compile error, \" should be applied
  const auto res5{"this is a line with quotes and (bracket)"};  // okay.
  const auto res22{R"(this is a line with "quotes" and (bracket))"};
  const auto res23{R"delim(this is a line with "quotes" and (bracket))delim"};
  std::cout << res2 << "\n";   // this is a line with "quotes" and (bracket)
  std::cout << res22 << "\n";  // this is a line with "quotes" and (bracket)
  std::cout << res23 << "\n";  // this is a line with "quotes" and (bracket)
}

void testRSLInFS() {
  // most easy and straitforward way to handle file path copied from file explorer
  const auto filePath{R"(C:\Users\Caiwang.Zuo\Desktop\life\slow-is-fast\.clang-format)"};
  const auto res1 = std::filesystem::exists(filePath);  // true

  const auto filePath1{"C:\\Users\\Caiwang.Zuo\\Desktop\\life\\slow-is-fast\\.clang-format"};
  const auto res2 = std::filesystem::exists(filePath1);  // true

  const auto filePath2{"C:/Users/Caiwang.Zuo/Desktop/life/slow-is-fast/.clang-format"};
  const auto res3 = std::filesystem::exists(filePath1);  // true

  int a = 1;
}

void test() {
  testBasic();
  testRSLInFS();
}

}  // namespace TestRawStringLiterals

namespace TestImediatelyInvokedFunctionExpression {
/*

Immediately Invoked Function Expressions (IIFE) in C++ and how they
can be utilized for benchmarking purposes on quick-bench.com. An IIFE is a programming pattern that involves defining
and immediately invoking a function, which can be useful for isolating scopes and initializing variables.

Key Points
What is an IIFE?
An Immediately Invoked Function Expression (IIFE) is a function that is defined and immediately called. This pattern is
commonly used in JavaScript, but it can also be applied in C++ for similar benefits, such as creating isolated scopes
and initializing complex data.

Syntax of IIFE in C++
In C++, you can create an IIFE using a lambda expression or a function object. The syntax typically involves defining a
lambda and immediately invoking it.
*/
void testIIFEWithLambda() {
  auto data = []() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    for (auto& x : vec) {
      x *= 2;
    }
    return vec;
  }();

  for (auto x : data) {
    std::cout << x << " ";  // Outputs: 2 4 6 8 10
  }
  std::cout << std::endl;
}

void test() {
  testIIFEWithLambda();
}

}  // namespace TestImediatelyInvokedFunctionExpression

namespace TestInheritance {
/*
Inheritance and Constructors
Constructors in derived classes need to initialize base class members. C++11 introduced new syntax for delegating
constructors and inheriting constructors.
*/
namespace TestInitializeBaseClass {
class Base {
  public:
  Base(int value) : _value(value) { std::cout << "Base constructor called! \n"; }

  public:
  int _value;
};

class Derived : public Base {
  public:
  Derived(int value, const std::string& name) : Base(value), _extra(name) {
    std::cout << "Derived constructor called! \n";
  }

  public:
  std::string _extra;
};

void test() {
  Base    b(3);
  Derived d(5, "lee");
  std::cout << "value: " << d._value << " value: " << d._extra << "\n";  // 5 lee
}

}  // namespace TestInitializeBaseClass

namespace TestInheritingConstructors {
/*
Inheriting constructors allow derived classes to inherit constructors from their base class, simplifying the constructor
definitions.
*/
class Base {
  public:
  Base(int value) : _value(value) { std::cout << "Base class constructor called!\n"; }

  public:
  int _value;
};

class Derived : public Base {
  public:
  using Base::Base;  // inherit constructors from Base class
  Derived() = delete;

  public:
  int _extra = 89;
};

void test() {
  Base    b(8);
  Derived d2(765);
  std::cout << ", val: " << d2._value << ", val: " << d2._extra << "\n";  // 765 89
}

}  // namespace TestInheritingConstructors

void test() {
  TestInitializeBaseClass::test();
  TestInheritingConstructors::test();
}

}  // namespace TestInheritance

namespace TestNoexcept {
/*
Summary
noexcept is used to declare that a function should not throw an exception, which if actually thrown would result
in a call to std::terminate.
The use of noexcept can help the compiler with optimizations, especially in mobile
semantics and standard library containers.Even in the absence of an explicit throw statement, some operations (such as
memory allocation failures, calls to functions that may throw exceptions, etc.) may result in an exception being
thrown.Understanding the usage and limitations of noexcept is important for writing efficient and robust modern C++
code!

The purpose of noexcept is to specify whether a function will throw an exception or not.It is primarily a compile-time
feature that tells the compiler that a function will not throw an exception, thus allowing the compiler to make more
optimizations.However, noexcept does not catch or prevent exceptions from being thrown at runtime.
*/

namespace TestNoexceptBasic {
/*
Basic Usage
The noexcept specifier can be applied to both regular and member functions, including constructors and destructors.
*/
void f() noexcept {
  // This function is declared as not throwing any exceptions.
}

class MyClass {
  public:
  void doSomething() noexcept {
    // This member function is declared as not throwing any exceptions.
  }
  MyClass() noexcept {
    // constructor declared as noexcept
  }

  ~MyClass() noexcept {
    // destructor declared as noexcept
  }
};

}  // namespace TestNoexceptBasic

namespace TestNoexceptOperator {
void mightThrowFunc() {
  throw std::runtime_error("A runtime error");
}

void noThrowFunc() noexcept {}

void test() {
  std::cout << "mightThrowFunc is noexcept: " << static_cast<bool>(noexcept(mightThrowFunc())) << "\n";  // false
  std::cout << "noThrowFunc is noexcept: " << static_cast<bool>(noexcept(noThrowFunc())) << "\n";        // true
}

}  // namespace TestNoexceptOperator

namespace TestBehaviour {
void func() noexcept {
  throw std::runtime_error("Exception in noexcept function");  // This will call std::terminate
}

void test() {
  try {
    func();  // call std::terminate. The program crashes
  } catch (const std::exception& e) {
    std::cout << "Caught: " << e.what() << std::endl;  // would not be hit
  }
}

}  // namespace TestBehaviour

void test() {
  TestNoexceptOperator::test();
  TestBehaviour::test();
}

}  // namespace TestNoexcept

namespace TestDisablingMoveFromConst {
/*
Moving from const objects can lead to subtle bugs and undefined behavior because moving typically modifies the source
object. To enforce immutability and ensure code correctness, it's important to disable moves from const objects.

Summary
Copy constructor: use const MyClass& parameter, because copy operation does not need to modify the source object.
So there is no difference between this two copy constructor versions: MyClass(MyClass&) and MyClass(const MyClass&)
Move constructor: use MyClass&& parameter, because the move operation needs to modify the source object.To
prevent moving const objects, a move constructor that accepts a const right-value reference can be deleted
(MyClass(const MyClass&&) = delete).

Why copy constructors don't need a const version?
Copy constructors are designed to handle const and non-const objects.Since the copy constructor does not modify the
source object, it can take a const reference argument.Therefore, the copy constructor works on both const and non-const
objects:
MyClass a; // non-const object const
MyClass b; // const object
MyClass a_copy = a; // Valid, uses copy constructorcopy constructor
MyClass b_copy = b; // Valid, uses copy constructor

*/

class MyClass {
  public:
  int* data;

  MyClass() : data(nullptr) {}

  // Move constructor
  MyClass(MyClass&& other) noexcept : data(other.data) { other.data = nullptr; }

  // Delete move constructor for const objects
  MyClass(const MyClass&&) = delete;

  // Move assignment operator
  MyClass& operator=(MyClass&& other) noexcept {
    if (this != &other) {
      delete data;
      data       = other.data;
      other.data = nullptr;
    }
    return *this;
  }

  // Delete move assignment operator for const objects
  MyClass& operator=(const MyClass&&) = delete;

  ~MyClass() { delete data; }
};

void test() {
  MyClass a;
  a.data = new int(42);

  MyClass b(std::move(a));  // Valid move

  const MyClass c;
  // MyClass d(std::move(c)); // Error: move constructor is deleted for const objects
}

}  // namespace TestDisablingMoveFromConst

namespace TestDelete {
namespace PreventingCopyAndMoveOperation {
class Noncopyable {
  public:
  Noncopyable()  = default;
  ~Noncopyable() = default;

  private:
  Noncopyable(const Noncopyable&)            = delete;
  Noncopyable& operator=(const Noncopyable&) = delete;
};

class Derived : public Noncopyable {
  public:
  Derived() = default;

  private:
  int _val = 10;
};

void test() {
  Derived d;
  // Derived       d1(d);  // compile error
  // Derived d1 = d; // compile error
}

}  // namespace PreventingCopyAndMoveOperation

namespace TestBasic {
/*
Preventing Unwanted Function Overloads
When you want to disallow specific overloads of a function, you can use = delete.
*/
void func(int) {
  std::cout << "int version" << std::endl;
}

void func(double) = delete;  // Disallow the double version

void test() {
  func(42);  // OK
  // func(3.14); // Error: use of deleted function 'void func(double)'
}

}  // namespace TestBasic

namespace TestDisablingImplicitConversion {
class MyClass {
  public:
  MyClass(int) {}
  MyClass(double) = delete;  // Disallow conversion from double to MyClass
};

void test() {
  MyClass obj1(42);  // OK
  // MyClass obj2(3.14); // Error: use of deleted function 'MyClass::MyClass(double)'
}

}  // namespace TestDisablingImplicitConversion

namespace SpecialUse {
/*
Singleton Pattern
Ensuring a class can only have one instance by deleting copy and move operations
*/
class Singleton {
  public:
  static Singleton& getInstance() {
    static Singleton instance;
    return instance;
  }

  private:
  Singleton() {}
  ~Singleton() {}

  // Delete copy constructor and copy assignment operator
  Singleton(const Singleton&)            = delete;
  Singleton& operator=(const Singleton&) = delete;

  // Delete move constructor and move assignment operator
  Singleton(Singleton&&)            = delete;
  Singleton& operator=(Singleton&&) = delete;
};

void test() {
  Singleton& s1 = Singleton::getInstance();
  // Singleton s2 = s1; // Error: use of deleted function 'Singleton::Singleton(const Singleton&)'
}
}  // namespace SpecialUse

namespace DeletingSpecialMemberFunction {
/*
Deleting Special Member Functions
You can delete special member functions such as the destructor if you want to prevent the object from being destroyed.
*/
class Immortal {
  public:
  Immortal() {}
  ~Immortal() = delete;  // Prevent destruction of objects
};

void test() {
  // Immortal obj; // Error: use of deleted function 'Immortal::~Immortal()'
}
}  // namespace DeletingSpecialMemberFunction

}  // namespace TestDelete

namespace TestEmplaceBack {
/*
In Episode 108 of C++ Weekly, Jason Turner discusses the emplace_back function in C++ and how it differs from the more
commonly used push_back when adding elements to containers like std::vector. Understanding emplace_back is crucial for
writing efficient and expressive C++ code, especially when dealing with complex objects.

Key Points
Overview of emplace_back
emplace_back is a member function of C++ standard containers like std::vector, std::deque, and std::list. It was
introduced in C++11 and allows for the direct construction of an object in place within the container, avoiding the need
for a temporary object.

Difference Between push_back and emplace_back
push_back:

push_back adds an existing object to the container.
It usually requires creating a temporary object before it is copied or moved into the container.

emplace_back:

emplace_back constructs the object directly in the memory allocated by the container, avoiding the creation of a
temporary object.

Benefits of Using emplace_back
Efficiency: emplace_back can be more efficient than push_back because it eliminates the need to create and potentially
copy or move temporary objects. Convenience: When dealing with complex objects, emplace_back allows you to pass the
constructor arguments directly, making the code cleaner and more readable. Direct Construction: Objects are constructed
directly in the container�s memory, which is particularly useful for types that are expensive to copy or move.

*/

class MyClass {
  public:
  MyClass(const MyClass& cl) {
    std::cout << "Copy constructor called!\n";
    _age  = cl._age;
    _name = cl._name;
  }

  MyClass(int age, const std::string& name) : _age(age), _name(name) { std::cout << "Default constructor called!\n"; };

  MyClass(MyClass&& cl) {
    std::cout << "Move constructor called!\n";
    _age  = cl._age;
    _name = std::move(cl._name);
  }

  private:
  int         _age;
  std::string _name;
};

void test() {
  std::vector<MyClass> vec;
  MyClass              mc(11, "jacky");
  vec.push_back(mc);  // Default constructor called! Copy constructor     called !
  std::cout << "line************************\n";
  std::vector<MyClass> vec2;
  vec2.emplace_back(MyClass(13, "lee"));  // Default constructor called! Copy constructor     called !
  std::cout << "new line ********************\n";
  std::vector<MyClass> vec1;
  vec1.emplace_back(45, "lance");  // Default constructor called!
}

}  // namespace TestEmplaceBack

namespace TestConstexprSTDOptionalSTDVariant {

constexpr std::optional<int> getOptionalValueInCompileTime(bool condition) {
  if (condition) {
    return 42;
  } else {
    return std::nullopt;  // std::nullopt is a constant of type std::nullopt_t. Used to indicate that an std::optional
                          // does not contain a value.
  }
}

constexpr std::variant<int, const char*> getVariantValueInCompileTime(bool condition) {
  if (condition) {
    return 42;
  } else {
    return "hello world";
  }
}

void test() {
  constexpr auto res1 = getOptionalValueInCompileTime(true);  // 42
  constexpr auto res2 = getVariantValueInCompileTime(false);  // hello world
  int            a    = 1;
}

}  // namespace TestConstexprSTDOptionalSTDVariant

namespace TestReferenceToPointerAndDoublePointers {

void testDoublePointer() {
  int   x    = 10;
  int*  ptr  = &x;
  int** dptr = &ptr;

  std::cout << "value of x: " << x << "\n";        // 10
  std::cout << "value of ptr: " << ptr << "\n";    // 000000AA563BF814 (memory address)
  std::cout << "value of dptr: " << dptr << "\n";  // 000000AA563BF838 (memory address)
}

namespace TestReferenceToPointer {
void allocateMemory(int*& p) {
  p = new int(100);  // dynamically allocate an int and assign to p
}

namespace TestHigherOderFunctions {
/*
Higher-Order Functions are functions that can take other functions as arguments or return them as results. This concept
is a key feature in functional programming but is also very useful in C++ for creating more flexible and reusable code.

Key Points
What is a Higher-Order Function?
A Higher-Order Function (HOF) is a function that:

Takes one or more functions as parameters, or
Returns a function as its result.
This allows you to create functions that can be used as building blocks for more complex operations, similar to how you
might compose objects in object-oriented programming.

Practical Use Cases for Higher-Order Functions
1.Callbacks: HOFs are commonly used in callback mechanisms where you pass a function to be called later, often in
response to an event or completion of a task.

2.Functional Composition: You can create HOFs that combine or modify other functions, such as chaining operations
together or applying transformations to data.

3.Lazy Evaluation: HOFs can be used to defer the execution of a function until its result is needed, allowing for more
efficient and responsive programs.

4.Decorators: You can create functions that wrap other functions to add additional behavior, such as logging or input
validation.

*/

namespace TestFunctionTakesAnotherFunctionAsParameter {
int square(int x) {
  return x * x;
}

void foo(int x, const std::function<int(int)>& func) {
  std::cout << "Result: " << func(x) << "\n";
}

void test() {
  foo(3, square);  // 9
}

}  // namespace TestFunctionTakesAnotherFunctionAsParameter

namespace TestReturnFunctionFromHOF {
std::function<int(int)> createMultiplier(int factor) {
  return [factor](int x) { return x * factor; };
}

void test() {
  auto timesThree = createMultiplier(3);
  std::cout << "3 times 4 equals: " << timesThree(4) << "\n";  // 12
}

}  // namespace TestReturnFunctionFromHOF

namespace TestFunctionalComposition {
int addOne(int x) {
  return x + 1;
}

int multiplyByTwo(int x) {
  return x * 2;
}

std::function<int(int)> composeFunction(const std::function<int(int)>& f, const std::function<int(int)>& g) {
  return [f, g](int x) { return f(g(x)); };
}

void test() {
  auto addThenMultiply = composeFunction(multiplyByTwo, addOne);
  std::cout << "Result: " << addThenMultiply(5) << "\n";  // 12
}

}  // namespace TestFunctionalComposition

void test() {
  TestFunctionTakesAnotherFunctionAsParameter::test();
  TestReturnFunctionFromHOF::test();
  TestFunctionalComposition::test();
}

}  // namespace TestHigherOderFunctions

namespace TestCallables {
/*
What Are Callables?
In C++, a callable is any object or entity that you can invoke using the function call syntax, which typically looks
like callable(). The callable entities in C++ include:

Functions: Regular functions defined with void func() {} or similar signatures.
Function Pointers: Pointers that point to functions.
Lambdas: Anonymous functions or closures that capture variables from their enclosing scope.
Functors (Function Objects): Instances of classes that overload the operator(), allowing the object to be used like a
function. Member Function Pointers: Pointers to non-static member functions of a class. std::function: A general-purpose
polymorphic wrapper for callable objects.
*/
namespace FunctionPointer {
void foo() {
  std::cout << "Hello world from function pointer\n";
}

void test() {
  void (*funcPtr)() = &foo;
  funcPtr();
}

}  // namespace FunctionPointer

namespace Lambda {
auto myLambda = []() { std::cout << "Hello world from lambda\n"; };

void test() {
  myLambda();
}

}  // namespace Lambda

namespace Functor {
class MyFunctor {
  public:
  void operator()() { std::cout << "Hello world from functor \n"; }
};

void test() {
  MyFunctor obj;
  obj();
}

}  // namespace Functor

namespace STDFunction {
void myFunction() {
  std::cout << "Hello world from std::function\n";
}

void test() {
  std::function<void()> func = myFunction;
  func();
}
}  // namespace STDFunction

namespace UsingCallablesInFunctionTemplate {
template <typename T, typename... Args>
void execute(T callable, Args... args) {
  callable(std::forward<Args>(args)...);
}

void myFoo(int val) {
  std::cout << "Hello world from function pointer and value: " << val << "\n";
}

auto customLambda = [](const std::string& name) { std::cout << "Hello world from lambda, name: " << name << "\n"; };

void test() {
  execute(FunctionPointer::foo);
  execute(Lambda::myLambda);
  execute(Functor::MyFunctor{});
  execute(STDFunction::myFunction);
  execute(myFoo, 99);
  execute(customLambda, "jacky");
}

}  // namespace UsingCallablesInFunctionTemplate

void test() {
  FunctionPointer::test();
  Lambda::test();
  Functor::test();
  STDFunction::test();
  std::cout << "split line***********************\n";
  UsingCallablesInFunctionTemplate::test();
}

}  // namespace TestCallables

void test() {
  TestCallables::test();
  // TestHigherOderFunctions::test();
  //  TestNoexcept::test();
  //   TestInheritance::test();
  //   TestImediatelyInvokedFunctionExpression::test();
  //    TestRawStringLiterals::test();
  //     TestGotoStatement::test();
  //     TestVariadicUsing::test();
  //      TestConstexprLambdaSupport::test();
  //       TestStartUsingDefaultMemberInitializer::test();
  //       TestFloatingLiteralsDefinition::test();
  //       TestStructuredBindings::test();
  //        TestIfAndSwitchInitStatements::test();
  //         TestStopUsingSTDEndl::test();
  //         TestVariadicExpansionWrapUp::test();
  //         TestFoldExpression::test();
  int* ptr = nullptr;
  allocateMemory(ptr);

  std::cout << *ptr << "\n";  // 100

  delete ptr;
}

}  // namespace TestReferenceToPointer

void test() {
  testDoublePointer();
  TestReferenceToPointer::test();
}

}  // namespace TestReferenceToPointerAndDoublePointers

namespace TestSTDToAddress {
/*
Key Points
What is std::to_address?
std::to_address is a function template provided in C++20 that returns the raw pointer (T*) from a pointer-like object.
It is especially useful in template programming, where you might be dealing with different types of pointers (raw
pointers, smart pointers, or iterators) and need to extract the underlying raw pointer for operations that require
direct pointer access.

The basic idea is to provide a uniform way to get the address of an object, regardless of the type of pointer or
iterator you're dealing with.

For Raw Pointers: If you pass a raw pointer to std::to_address, it simply returns the pointer itself.

For Smart Pointers: If you pass a smart pointer (e.g., std::unique_ptr, std::shared_ptr), std::to_address calls the
smart pointer's get() member function to obtain the raw pointer.

For Iterators: If you pass an iterator, std::to_address dereferences the iterator and takes the address of the
dereferenced value.

*/
template <typename Ptr>
void printAddress(const Ptr& ptr) {
  std::cout << "Address: " << std::to_address(ptr) << "\n";
}

void test() {
  int                  x      = 10;
  int*                 rawPtr = &x;
  std::unique_ptr<int> uptr   = std::make_unique<int>(20);
  std::vector<int>     vec{60, 20, 30};
  auto                 it = vec.begin();

  printAddress(rawPtr);  // 000000B9691BF774
  printAddress(uptr);    // 0000027D737E5C70
  printAddress(it);      // 0000027D737F1FE0
}

}  // namespace TestSTDToAddress

namespace TestInline {
/*
What is an inline Variable?
Before C++17, defining a global or static variable in a header file could lead to multiple definition errors if the
header was included in more than one translation unit. To avoid this, the variable would typically be declared extern in
the header file and defined in a single source file.

C++17 introduced inline variables, which allow you to define a variable in a header file without violating the One
Definition Rule (ODR). This means that the variable can be defined in multiple translation units, and the linker will
ensure that all these definitions refer to the same object.

Example: Global Constants
Before C++17, defining a global constant in a header file could lead to issues if not handled carefully:

// global_definitions.h
extern const int globalConst;

// global_definitions.cpp
#include "global_definitions.h"
const int globalConst = 42;
With C++17, you can define it directly in the header file using inline:

// global_definitions.h
inline const int globalConst = 42;
Now, globalConst can be included in multiple translation units without causing linker errors.

*/

}

void test() {
  TestPtrVariable::test();

  //TestBitShift::test();

  // TestSTDToAddress::test();

  // TestReferenceToPointerAndDoublePointers::test();
  // TestConstexprSTDOptionalSTDVariant::test();
  // TestEmplaceBack::test();
  // TestNoexcept::test();
  //  TestInheritance::test();
  //  TestImediatelyInvokedFunctionExpression::test();
  //   TestRawStringLiterals::test();
  //    TestGotoStatement::test();
  //    TestVariadicUsing::test();
  //     TestConstexprLambdaSupport::test();
  //      TestStartUsingDefaultMemberInitializer::test();
  //      TestFloatingLiteralsDefinition::test();
  //      TestStructuredBindings::test();
  //       TestIfAndSwitchInitStatements::test();
  //        TestStopUsingSTDEndl::test();
  //        TestVariadicExpansionWrapUp::test();
  //        TestFoldExpression::test();
}