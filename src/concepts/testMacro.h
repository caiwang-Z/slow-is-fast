#pragma once
#include <iostream>
#include <source_location>
#include <string>
#include "utility.h"
using UtilityNameSpace::myLog;
using UtilityNameSpace::splitLine;

namespace TestStdSourceLocation {
/*
std::source_location is a new lightweight type added to C++20, which is used to "capture" the location of the source
code (filename, line number, column number, function name, etc.) at the point of call at compile time, and is commonly
used in logging, assertion, and debugging tool scenarios.

Summary:
    1.std::source_location provides a uniform, automated way of obtaining information about the location of the source
code at the point of call.

    2. The most common pattern is to use it as an argument to a function with a default value: =
    std::source_location::current().

    3. It can be used in logging, assertion, debugging tools, performance tracing, and other scenarios, saving you the
trouble of manually writing macros such as __FILE__ and __LINE__.
*/

void log_debug(const std::string& msg, const std::source_location& loc = std::source_location::current()) {
  std::cout << "[" << loc.file_name() << ":" << loc.line() << "] " << loc.function_name() << "(): " << msg << "\n";
}

void foo(int x) {
  log_debug("Entering foo, x = " + std::to_string(x));
  // ...
  log_debug("Leaving foo");
}

void test() {
  log_debug("Program starts....");
  foo(42);
  log_debug("Program ends....");
}

}  // namespace TestStdSourceLocation

namespace TestMacro {
// 1. defining func with macro
/* with backsplash \ at the end of the line without space
allows you to type multiplines for one macro definition
#define MAIN int main()\
{\
std::cin.get();\

}
*/

// 2. define the macro
#define PR_DEBUG 1

// 3. use the macro in a conditional compilation check
#if PR_DEBUG == 1
#define LEVEL 2
#endif
}  // namespace TestMacro

namespace TestCPPPredefinedMacros {
// The compiler automatically defines this macro when processing C++ code, so the programmer doesn't need to define it
// manually
#if __cplusplus == 202002L
std::cout << "C++20" << std::endl;

#endif
void test() {
  std::cout << "Current file: " << __FILE__ << std::endl;
  std::cout << "Current line: " << __LINE__ << std::endl;
  std::cout << "Current func: " << __func__ << std::endl;
}

}  // namespace TestCPPPredefinedMacros

namespace TestOneMacroMultipleItems {

#define GENERIC_ITEM STRING_ITEM, INT_ITEM, FLOAT_ITEM, ENUM_ITEM

void testSwithCase() {
  enum NewItems {
    custom_item,
    new_item,
    GENERIC_ITEM

  };

  int i = 0;

  NewItems newItem{NewItems::FLOAT_ITEM};

  switch (newItem) {
    case STRING_ITEM:
      i = 1;
    case INT_ITEM:
      i = 2;
    case custom_item:
      i = 3;
    case new_item:
      i = 4;
    case FLOAT_ITEM:
      i = 5;
    case ENUM_ITEM:
      i = 6;
    default:
      break;
  }

  myLog(i);  // 5
}

void test() {
  testSwithCase();
}

}  // namespace TestOneMacroMultipleItems

namespace TestMacroCreateComplexDataStructure {
/*
In C or C++, macro definitions are made through a preprocessor, as opposed to namespace scopes.
Macros are global and, once defined, can be used throughout the entire compilation unit
(usually a source file and its contained header files).
Thus, if a macro is defined in namespace a in one file, it is accessible in namespace b in the same file.

*/

#define CUSTOM_GENERATED_INFOS                                                                                         \
  {STRING_ITEM, 0X40000000, 80, false}, {INT_ITEM, 0X50000000, 90, true}, {FLOAT_ITEM, 0X60000000, 100, false}, {      \
    ENUM_ITEM, 0X90000000, 199, true                                                                                   \
  }

enum CustomEnum { GENERIC_ITEM };  // must transter macro to the enum struct

struct RegisterInfo {
  unsigned id;
  uint64_t address;
  size_t   size;
  bool     var_sized;
};

void test() {
  RegisterInfo infoArrays[] = {CUSTOM_GENERATED_INFOS};
  myLog(infoArrays[1].id);  // 1
  myLog(infoArrays[3].id);  // 3
  myLog(infoArrays[2].id);  // 2
}

}  // namespace TestMacroCreateComplexDataStructure

void test() {
#if LEVEL == 1
  int a = 1;
#elif LEVEL == 2
  int a = 2;
#endif
  // TestCPPPredefinedMacros::test();
  // splitLine();
  // TestOneMacroMultipleItems::test();
  // splitLine();
  // TestMacroCreateComplexDataStructure::test();
  TestStdSourceLocation::test();
}
