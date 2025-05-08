#pragma once

#include <array>
#include <iostream>

namespace TestLocalStaticInFunction {
void Function() {
  // int i = 0; // output: 1, 1, 1
  static int i = 0;  // output: 1, 2, 3
  i++;
  std::cout << i << std::endl;
}

void test() {
  Function();
  Function();
  Function();
}

}  // namespace TestLocalStaticInFunction

namespace TestLegitInNamespace {
/*
Allowed in Namespace Scope:
Declaration and initialization of variables
Definition of functions and methods
Declaration of new types (like structs, classes, enums)
Declaration of other namespaces

Not Allowed in Namespace Scope:
Executing operations on variables (like assignment, addition, modification)
Calling functions that are not mere declarations
*/
int                b   = 100;
static std::string str = "like you";
// str = str + "some";  // not allowed

}  // namespace TestLegitInNamespace

namespace TestLocalStaticInSingleton {
// without static
class Singleton {
  private:
  static Singleton& get() {
    static Singleton instance;  // extending lifetime of this to be forever
    return instance;
  }

  void hello() {}
};

void test() {}
}  // namespace TestLocalStaticInSingleton

namespace TestStaticScopeAccessAndLifeTimeInClassFunction {
// TODO static visiblity in function and class, static declaration in class, not in definition
}

void test() {
  TestStaticArrayData::test();
}