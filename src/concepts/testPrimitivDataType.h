#pragma once
#include <iostream>
#include <string_view>
#include <type_traits>

namespace MakeStringFaster {
static uint32_t s_AllocCount = 0;

/*
C++ states that operator new and operator delete must be defined in the global namespace and cannot be declared as
static functions in classes or other namespaces.This is because these operators are part of the global operations that
are responsible for allocating and freeing memory.
*/
//void* operator new(size_t size) {
//  s_AllocCount++;
//  std::cout << "Allocating " << size << " bytes\n";
//  return malloc(size);
//} // compile error

void printName(std::string_view name) {
  std::cout << name << std::endl;
  /*
  * No need to printName(const std::string_view name)
  Therefore, adding const to a std::string_view parameter is generally redundant unless you want to enforce that the
  string_view object itself cannot be redirected within the function, which is a rare requirement.
  */
  /*
  std::string_view is a preferred choice over std::string for function parameters when you don't need to modify or own
  the string data. It avoids the overhead of copying the string data, which can lead to performance improvements,
  especially in functions that are called frequently or with large strings.
  */
}

void test() {
  // In C++, the Small String Optimization (SSO) is a technique used by implementations of std::string to store small
  // strings directly within the string object itself, avoiding the need for dynamic memory allocation. std::string name
  // = "Ya";  // 0 allocations because of Small String Optimization (SSO)
  std::string name = "Yan Chernikov";  // 1 allocations
#if 1
  std::string firstName = name.substr(0, 3);
  std::string lastName  = name.substr(4, 9);  // 3 allocation
#else
  std::string_view firstName{name.c_str(), 3};
  std::string_view lastName{name.c_str() + 4, 9};  // 1 allocation
#endif

  std::cout << s_AllocCount << " allocations" << std::endl;
}

}  // namespace MakeStringFaster

namespace TestTypeTraits {
/*
In the C++ standard library, there are a number of type traits tools similar to std::is_integral.These tools are mainly
defined in the <type_traits> header file and are used to detect various characteristics of types.Here are some commonly
used type traits and their uses
*/

void test() {
  std::cout << std::boolalpha;  // Print bool values as true/false

  // Check if the type is integral
  std::cout << "Is integral (int): " << std::is_integral<int>::value << "\n";      // true
  std::cout << "Is integral (float): " << std::is_integral<float>::value << "\n";  // false

  // Check if the type is floating point
  std::cout << "Is floating point (float): " << std::is_floating_point<float>::value << "\n";  // true
  std::cout << "Is floating point (int): " << std::is_floating_point<int>::value << "\n";      // false

  // Check if the type is arithmetic (integral or floating point)
  std::cout << "Is arithmetic (int): " << std::is_arithmetic<int>::value << "\n";                  // true
  std::cout << "Is arithmetic (float): " << std::is_arithmetic<float>::value << "\n";              // true
  std::cout << "Is arithmetic (std::string): " << std::is_arithmetic<std::string>::value << "\n";  // false

  // Check if the type is signed
  std::cout << "Is signed (int): " << std::is_signed<int>::value << "\n";                    // true
  std::cout << "Is signed (unsigned int): " << std::is_signed<unsigned int>::value << "\n";  // false

  // Check if the type is unsigned
  std::cout << "Is unsigned (unsigned int): " << std::is_unsigned<unsigned int>::value << "\n";  // true
  std::cout << "Is unsigned (int): " << std::is_unsigned<int>::value << "\n";                    // false

  // Check if the type is an array
  std::cout << "Is array (int[]): " << std::is_array<int[]>::value << "\n";  // true
  std::cout << "Is array (int): " << std::is_array<int>::value << "\n";      // false

  // Check if the type is a pointer
  std::cout << "Is pointer (int*): " << std::is_pointer<int*>::value << "\n";  // true
  std::cout << "Is pointer (int): " << std::is_pointer<int>::value << "\n";    // false

  // Check if the type is a reference
  std::cout << "Is reference (int&): " << std::is_reference<int&>::value << "\n";  // true
  std::cout << "Is reference (int): " << std::is_reference<int>::value << "\n";    // false

  // Check if the type is a member pointer
  struct S {
    int member;
  };
  std::cout << "Is member pointer (int S::*): " << std::is_member_pointer<int S::*>::value << "\n";  // true
  std::cout << "Is member pointer (int*): " << std::is_member_pointer<int*>::value << "\n";          // false

  // Check if the type is void
  std::cout << "Is void (void): " << std::is_void<void>::value << "\n";  // true
  std::cout << "Is void (int): " << std::is_void<int>::value << "\n";    // false

  // Check if the type is nullptr_t
  std::cout << "Is nullptr_t (std::nullptr_t): " << std::is_null_pointer<std::nullptr_t>::value << "\n";  // true
  std::cout << "Is nullptr_t (int*): " << std::is_null_pointer<int*>::value << "\n";                      // false

  // Check if the type is an enum
  enum E { A, B };
  std::cout << "Is enum (E): " << std::is_enum<E>::value << "\n";      // true
  std::cout << "Is enum (int): " << std::is_enum<int>::value << "\n";  // false

  // Check if the type is constructible
  std::cout << "Is constructible (std::string, const char*): " << std::is_constructible<std::string, const char*>::value
            << "\n";                                                                                     // true
  std::cout << "Is constructible (int, double): " << std::is_constructible<int, double>::value << "\n";  // true
  std::cout << "Is constructible (int, std::string): " << std::is_constructible<int, std::string>::value
            << "\n";  // false

  // Check if the type is default constructible
  std::cout << "Is default constructible (int): " << std::is_default_constructible<int>::value << "\n";  // true
  std::cout << "Is default constructible (std::string): " << std::is_default_constructible<std::string>::value
            << "\n";  // true

  // Check if the type is copy constructible
  std::cout << "Is copy constructible (int): " << std::is_copy_constructible<int>::value << "\n";  // true
  std::cout << "Is copy constructible (std::string): " << std::is_copy_constructible<std::string>::value
            << "\n";  // true

  // Check if the type is move constructible
  std::cout << "Is move constructible (int): " << std::is_move_constructible<int>::value << "\n";  // true
  std::cout << "Is move constructible (std::string): " << std::is_move_constructible<std::string>::value
            << "\n";  // true

  // Check if the type is assignable
  std::cout << "Is assignable (int&, int): " << std::is_assignable<int&, int>::value << "\n";    // true
  std::cout << "Is assignable (int&, int&): " << std::is_assignable<int&, int&>::value << "\n";  // true
  std::cout << "Is assignable (int, int): " << std::is_assignable<int, int>::value << "\n";      // false

  // Check if the type is copy assignable
  std::cout << "Is copy assignable (int): " << std::is_copy_assignable<int>::value << "\n";                  // true
  std::cout << "Is copy assignable (std::string): " << std::is_copy_assignable<std::string>::value << "\n";  // true

  // Check if the type is move assignable
  std::cout << "Is move assignable (int): " << std::is_move_assignable<int>::value << "\n";                  // true
  std::cout << "Is move assignable (std::string): " << std::is_move_assignable<std::string>::value << "\n";  // true

  // Check if the type is destructible
  std::cout << "Is destructible (int): " << std::is_destructible<int>::value << "\n";                  // true
  std::cout << "Is destructible (std::string): " << std::is_destructible<std::string>::value << "\n";  // true
}

}  // namespace TestTypeTraits

void test() {
  MakeStringFaster::test();
  TestTypeTraits::test();
}