#pragma once
#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <variant>
#include <vector>
#include "utility.h"

using namespace UtilityNameSpace;
using UtilityNameSpace::Splitter;

namespace TestFinalKeyword {
/*
1. On a class
Marking an entire class final prevents any other class from inheriting from it.
*/
namespace PreventInherit {
class Base final {
  void foo(){};
};

// class Derived : public Base {};
//
// }
//  Error: compile error. Base class could not be inherited.

/*
2.On a virtual member function
Marking a virtual function final prevents any further overrides of that function in derived classes.
*/
namespace PreventOverrideMemberFunction {
class Base {
  public:
  virtual void f();
  virtual void g() final;
};

class Derived : public Base {
  public:
  void f() override { std::cout << "override base funciton"; }

  // void g() override {
  //// do something
  //} // Compile Error!. Not allowed to override
};

}

}

namespace TestFriendKeyWord {
/*
1. Friend Functions
A friend function is a non-member function that has been given access to a class’s private and protected members.

Declaration
Inside the class definition, you prefix the function prototype with the keyword friend:

Where to put it: The friend declaration can appear under public, protected, or private sections, but it’s common to put
it under public so it’s visible to users of the class.

Access: Friend functions can read or modify private/protected data, just like member functions.

Not inherited: Friendship is not inherited. If Box is a friend of Foo, subclasses of Box are not automatically friends
of Foo.
*/
namespace TestFriendFunctions {
class Box {
  public:
  Box(double width) : _width(width){};

  friend void printBoxWidth(const Box& box);

  private:
  double _width;
};

void printBoxWidth(const Box& box) {
  std::cout << "Box width: " << box._width << std::endl;
}

void test() {
  Box b(99);
  printBoxWidth(b);
}

}  // namespace TestFriendFunctions

namespace TestfriendOperatorOverloading {
class DummyNumber {
  public:
  DummyNumber(int a, int b) : _left(a), _right(b){};
  void               printValue() const { std::cout << "Left: " << _left << ", Right: " << _right << std::endl; }
  friend DummyNumber operator+(const DummyNumber& a, const DummyNumber& b);

  private:
  int _left;
  int _right;
};

DummyNumber operator+(const DummyNumber& a, const DummyNumber& b) {
  return DummyNumber(a._left + b._left, a._right + b._right);
}

void test() {
  DummyNumber n1(10, 100), n2(9, 99);
  DummyNumber n3 = n1 + n2;
  n3.printValue();
}

}  // namespace TestfriendOperatorOverloading

/*
2. Friend Classes
A friend class is one whose member functions all have access to another class’s private and protected members.

Declaration
Inside the granting class:
Granularity: Declaring a friend class grants access to all of its members. If you only want specific functions, prefer
friend functions or friend member functions.

Not reciprocal: If Car names Engine as friend, Engine can access Car’s internals, but Car does not gain any special
access to Engine.

Not transitive or inherited:

If A is friend of B, and B is friend of C, A is not automatically friend of C.

If Derived inherits from Base, and Base has made Friend a friend, Derived does not automatically grant friendship to
Friend.

*/

namespace TestFriendClass {
class Engine;

class Car {
  public:
  Car(int horsePower) : _hp(horsePower){};

  void showInfo() const { std::cout << "This car has " << _hp << " horse power" << std::endl; }

  friend Engine;

  private:
  int _hp;
};

class Engine {
  public:
  Engine(){};

  void tune(Car& car, int newHorsePower) { car._hp = newHorsePower; }
  void showHorsePower(const Car& car) { std::cout << "This car has " << car._hp << " horse power." << std::endl; }
};

void test() {
  Car car(100);
  car.showInfo();
  Engine e;
  e.tune(car, 200);
  car.showInfo();
}

}  // namespace TestFriendClass

/*
3. Friend Member Functions
You can also grant friendship to a member of another class:

*/
namespace TestFriendMemberFunctions {
// 1) Forward‐declare both the class and the member signature:
class Car;  // so Engine can refer to Car
class Engine {
  public:
  void tune(Car& car, int newHorsePower);  // <-- declare it here
  void showHorsePower(const Car& car) const;
};

// 2) Now define Car and friend that member:
class Car {
  public:
  Car(int horsePower) : _hp(horsePower){};

  void        showInfo() const { std::cout << "This car has " << _hp << " horse power" << std::endl; }
  friend void Engine::tune(Car&, int);
  // …
  private:
  int _hp;
};

// 3) Finally, define Engine::tune:
void Engine::tune(Car& car, int newHorsePower) {
  car._hp = newHorsePower;  // now OK
}

void Engine::showHorsePower(const Car& car) const {
  // std::cout << "This car has " << car._hp << " horse power." << std::endl; // compile error. Because this member
  // function has no access to private member of Car class
}

void test() {
  Car car(100);
  car.showInfo();
  Engine e;
  e.tune(car, 200);
  car.showInfo();
}

}  // namespace TestFriendMemberFunctions

/*
4. Key Characteristics of Friendship
4.1 One-way
If X is friend of Y, Y is not friend of X unless explicitly declared.

4.2 Not inherited
Subclasses do not inherit friends of their base classes.

4.3 Not transitive
Friendship does not chain through classes/functions.

4.4 Declared in granting class
A class grants friendship; it never “requests” it.

5. When to Use (and When Not)
Pros
Tighter coupling for very close classes or helper functions.

Cleaner interfaces for operators and non-member algorithms.

Selective access without making everything public.

Cons
Breaks encapsulation — internal details leak.

Harder to maintain — lots of friends can indicate poor design.

Increases coupling — changes to private members may force updates in friends.

Best Practices
Minimize use: Only when there’s a strong rationale (e.g. symmetric operators, performance-critical code).

Favor non-friend alternatives:

Public getters/setters (when safe).

Pimpl idiom or adapter patterns.

Document why friendship exists and what can be safely assumed.

6. Summary
Friend functions let you give a standalone function special access to a class’s private/protected members.

Friend classes let an entire class access another’s internals.

Friendship should be used judiciously: it’s a powerful tool, but overuse leads to tangled, hard-to-maintain code.

By understanding and controlling friendship in C++, you can strike the right balance between encapsulation and the
practical need for tightly-coupled operations.

*/

void test() {
  TestFriendFunctions::test();
  TestfriendOperatorOverloading::test();
  TestFriendClass::test();
  TestFriendMemberFunctions::test();
}

}  // namespace TestFriendKeyWord

namespace TestOptionalKeyWord {
// std::optional to store data which may or may not be present
std::optional<std::string> ReadFileAsString(const std::string& filepath) {
  std::ifstream stream(filepath);
  if (stream) {
    std::string result;
  }
  return {};
}

void test() {
  std::optional<std::string> data  = ReadFileAsString("data.txt");
  std::string                value = data.value_or("No present");
  std::cout << value << std::endl;

  if (data.has_value()) {
    std::cout << "File read sucessfully.";
  } else {
    [[maybe_unused]] std::string& string = *data;  // like smart pointer
    std::cout << "File could not be opened.";
  }
}

}  // namespace TestOptionalKeyWord

namespace TestVariantKeyWord {
enum class ErrorCode { None = 0, NotFound = 1, NoAccess = 2 };

std::variant<std::string, ErrorCode> ReadFileAsString() {
  return {};
}

void test() {
  std::variant<std::string, int> data;
  data = "cherno";
  std::cout << std::get<std::string>(data) << "\n";
  [[maybe_unused]] auto* value = std::get_if<std::string>(&data);

  data = 2;
  std::cout << std::get<int>(data) << "\n";
  [[maybe_unused]] auto* value1 = std::get_if<int>(&data);
}

}  // namespace TestVariantKeyWord

namespace TestNullKeyWord {
class Entity {
  public:
  Entity() = default;

  const std::string& printName() { return _name; }

  void printType() { std::cout << "Entity\n" << std::endl; }

  public:
  Entity*     _parent;
  std::string _name;
};

void test() {
  Entity* en = nullptr;
  std::cout << "Line 1 *****************************************\n";
  en->printType();
  std::cout << "Line 2 *****************************************\n";
  [[maybe_unused]] uint64_t offset = offsetof(Entity, _name);  // 8, the address of nullptr (NULL in c) is 0x00000000000
  std::cout << "name of entity: " << en->printName();          // Exception thrown: read access violation.
  // **this** was 0x8. (check offset of _name in class Entity)
}

}  // namespace TestNullKeyWord
namespace TestCplusPlusAttribute {
namespace TestNodiscard {
// The [[nodiscard]] attribute is used to warn the programmer that the return value of a function should be used.
// If the function's return value is ignored, the compiler will generate a warning.

[[nodiscard]] int sum(int a, int b) {
  return a + b;
}

[[nodiscard]] char* getNewMessage() {
  char* ms = new char[100];
  strcpy(ms, "Hello World.");
  return ms;
}

void testAttributeNoDiscard() {
  SIFSplit;
  // sum(10, 20);  // warning C4834: discarding return value of function with 'nodiscard' attribute
  [[maybe_unused]] auto res = sum(10, 20);  // now warning
  // There is a memory leak produced because the returned value was not stored anywhere and
  // there is no way to deallocate its memory.
  // getNewMessage();  // warning C4834: discarding return value of function with 'nodiscard' attribute
  [[maybe_unused]] auto res1 = getNewMessage();  // no warning
}

}  // namespace TestNodiscard

namespace TestDeprecated {
// The [[deprecated]] attribute is used to mark entities (e.g. functions, variables, types, etc.) that are not
// recommended for use. The compiler generates a warning when using entities marked as [[deprecated]].

[[deprecated("This function will be removed in the future. Use newFunc instead")]] void oldFunc(){};

void newFunc(){};

void testAttributeDeprecated() {
  SIFSplit;
  // oldFunc();  // warning C4996: 'TestCplusPlusAttribute::TestDeprecated::oldFunc': This function will be removed in
  // the future. Use newFunc instead
  newFunc();
}

}  // namespace TestDeprecated

namespace TestFallThrough {
// This helps avoid unexpected control flow errors and makes the code more readable.
void process(int value) {
  switch (value) {
    case 0:
      std::cout << "case 0 \n";
      [[fallthrough]];
    case 1:
      std::cout << "case 1 \n";
      [[fallthrough]];
    case 2:
      std::cout << "case 2 \n";
      break;
    default:
      std::cout << "Default case \n";
      break;
  }
}

void testAttributeFallThrough() {
  SIFSplit;
  process(0);  // case 0; case 1; case 2
}

}  // namespace TestFallThrough

namespace TestLikelyAndUnlikely {
// These attributes help the compiler optimize by suggesting that a branch is more or less likely to be executed.
// These attributes were introduced in C++20.
void testAttributeLikelyAndUnlikelyInIfElse() {
  SIFSplit;
  int x = 10;

  if (x > 1) [[likely]] {
    std::cout << "x is greater than 1\n";
  } else if (x == 1) [[unlikely]] {
    std::cout << "x equals 1 \n";
  } else {
    std::cout << "x is smaller that 1 \n";
  }
}

void testAttributeLikelyAndUnlikelyInSwitch() {
  SIFSplit;
  int x = 10;
  switch (x) {
    case 0:
      std::cout << "case 0 \n";
      [[unlikely]];
      break;
    case 5:
      std::cout << "case 5 \n";
      [[unlikely]];
      break;
    case 10:
      std::cout << "case 10 \n";
      [[likely]];
      break;
    default:
      std::cout << "default case \n";
      break;
  }
}

}  // namespace TestLikelyAndUnlikely

namespace TestNoReturn {
// The [[noreturn]] attribute is used to mark functions that do not return.
// This is typically used for functions that throw an exception or terminate the program.
[[noreturn]] void fatalError(const char* msg) {
  throw "fatal error";
}

void testAttributeNoReturn() {
  SIFSplit;

  try {
    fatalError("Something went wrong");

  } catch (const char* msg) {
    std::cout << "Exception caught: " << msg << "\n";
  }
}
}  // namespace TestNoReturn

namespace TestMaybeUnused {
void foo([[maybe_unused]] int a, int b, int c, int d) {
#ifdef CONDITION
  std::cout << "a: " << a << ",b: " << b << ", c: " << c << ", d: " << d << "\n";
#else
  std::cout << "b: " << b << ", c: " << c << ", d: " << d << "\n";
#endif
};

void testAttributeMaybeUnused() {
  SIFSplit;
  [[maybe_unused]] int a = 10;  // no warning
  foo(1, 2, 3, 4);
}

}  // namespace TestMaybeUnused

void test() {
  TestNodiscard::testAttributeNoDiscard();
  TestDeprecated::testAttributeDeprecated();
  TestFallThrough::testAttributeFallThrough();
  TestLikelyAndUnlikely::testAttributeLikelyAndUnlikelyInIfElse();
  TestLikelyAndUnlikely::testAttributeLikelyAndUnlikelyInSwitch();
  TestNoReturn::testAttributeNoReturn();
  TestMaybeUnused::testAttributeMaybeUnused();
}
}  // namespace TestCplusPlusAttribute

// 1. Enabling Dynamic Dispatch
namespace TestVirtualInClass {
// 1.1 Non-virtual member functions are bound at compile time (static binding):
class Entity {
  public:
  std::string getName() { return "Entity"; }
};

class Player : public Entity {
  public:
  Player(const std::string& name) : _name(name) {}
  std::string getName() { return _name; }

  private:
  std::string _name;
};

void testPlayer() {
  Entity* en  = new Entity();
  Entity* pl  = new Player("Player");
  Player* pl2 = new Player("Player2");
  std::cout << en->getName() << "\n";   // Entity
  std::cout << pl->getName() << "\n";   // Entity
  std::cout << pl2->getName() << "\n";  // Player2
  delete en;
  delete pl;
  delete pl2;
}

namespace NSwithOverride {
// 1.2 Virtual member functions are bound at runtime (dynamic binding):
class EntityNew {
  public:
  virtual std::string getName() { return "Entity"; }
};

class PlayerNew : public EntityNew {
  public:
  PlayerNew(const std::string& name) : _name(name) {}
  std::string getName() override { return _name; }

  private:
  std::string _name;
};

void testPlayer() {
  EntityNew* en  = new EntityNew();
  EntityNew* pl  = new PlayerNew("Player");
  PlayerNew* pl2 = new PlayerNew("Player2");
  std::cout << en->getName() << "\n";   // Entity
  std::cout << pl->getName() << "\n";   // Player
  std::cout << pl2->getName() << "\n";  // Player2
  delete en;
  delete pl;
  delete pl2;
}

}  // namespace NSwithOverride

void test() {
  testPlayer();
  splitLine();
  NSwithOverride::testPlayer();
}

}  // namespace TestVirtualInClass

namespace AbstractBaseClassAndPureVirtualFunction {
// 2. Abstract Base Classes and Pure Virtual Functions
// A pure virtual function declares an interface without implementation:
// Shape cannot be instantiated directly. It serves as an interface that derived classes must implement.
class Shape {
  public:
  virtual double area() const = 0;  // pure virtual: no default implementation
  virtual ~Shape(){};               // virtual destructor ensures proper cleanup
};

class Circle : public Shape {
  public:
  Circle(double r) : _radius(r){};
  double area() const override { return 3.1415926 * _radius * _radius; }

  private:
  double _radius;
};

class Square : public Shape {
  public:
  Square(double length) : _length(length){};

  double area() const override { return _length * _length; }

  private:
  double _length;
};

void test() {
  Shape* circle = new Circle(1.0);
  Shape* sq     = new Square(2.0);

  circle->area();
  sq->area();  // 4

  delete circle;
  delete sq;
}

}  // namespace AbstractBaseClassAndPureVirtualFunction

namespace VirtualInClassDestructor {
// 3. Virtual Destructors for Safe Cleanup
// If you delete a derived object through a base-class pointer, the base’s destructor must be virtual, or the derived
// destructor won’t run:
class Entity {
  public:
  Entity() { std::cout << "Entity construcor called\n"; }

  ~Entity() { std::cout << "Entity destructor called\n"; }
};

class Player : public Entity {
  public:
  Player(const std::string& name) : _name(name) { std::cout << _name << " constructor called\n"; }
  ~Player() { std::cout << _name << " destructor called\n"; }

  private:
  std::string _name;
};

void test() {
  Entity* en = new Player("Player");
  delete en;  // Entity construcor called // Player constructor called // Entity destructor called
}

class Entity1 {
  public:
  Entity1() { std::cout << "Entity1 construcor called\n"; }

  virtual ~Entity1() { std::cout << "Entity1 destructor called\n"; }
};

class Player1 : public Entity1 {
  public:
  Player1(const std::string& name) : _name(name) { std::cout << _name << " constructor called\n"; }
  ~Player1() { std::cout << _name << " destructor called\n"; }

  private:
  std::string _name;
};

void test1() {
  Entity1* en = new Player1("Player1");
  delete en;  // Entity1 construcor called // Player1 constructor called //Player1 destructor called  // Entity1
              // destructor called
}

void testAll() {
  test();
  splitLine();
  test1();
}

}  // namespace VirtualInClassDestructor

namespace OverrideAndNameHiding {
// 4. Override vs. Name Hiding
// 4.1 Override: A derived class method with the exact same signature replaces the base virtual:
class Base1 {
  public:
  virtual void foo(int){};

  virtual ~Base1(){};
};

class Derived1 : public Base1 {
  public:
  void foo(int) override{};  // Correctly overrides Base::f(int)
  // void f(double) override {}  // Error: signature doesn’t match
};

// 4.2 Name Hiding: A derived method with the same name but different parameters hides all base overloads:
class Base2 {
  public:
  virtual void f(int) {}
  virtual ~Base2(){};
};

class Derived2 : public Base2 {
  public:
  void f(double) {}  // Hides Base::f(int), even though it’s virtual
};

/*
5. Memory and Performance Overhead
Using virtual functions adds:

    1.One extra pointer per object (the vptr).
    2. One indirection on each virtual call (lookup in vtable).

In most cases, this overhead is tiny compared to the benefits of flexible, safe interfaces.

Summary:

    1.Mark a member virtual to enable dynamic binding at runtime.
    2. Use pure virtuals (= 0) to define abstract interfaces.
    3. Always declare a virtual destructor in polymorphic base classes.
    4. Use override in derived classes to let the compiler check your intent.
    5. Accept the small runtime and memory cost in exchange for powerful polymorphism.
*/

}  // namespace OverrideAndNameHiding

namespace TestAlignasInStruct {
/*
*
The alignas(uint64_t) specifier ensures that the structure is properly
aligned for performance or hardware requirements.

*/
struct alignas(uint64_t) transferParams {
  const char* fileName;
  const char* fileType;
};

}  // namespace TestAlignasInStruct

namespace TestDecltype {
// #include <type_traits>

/*
Note 1: std::is_same is a template class that has two template parameters.In this example, the template parameters are
decltype(x) and int. std::is_same's main member is a static constant value

Note 2: static_assert is a compile-time assertion mechanism introduced by C++11 to check whether certain conditions are
true during compilation. If the condition is false, the compiler generates an error and displays the supplied error
message.
*/
void test() {
  int        a = 42;
  const int& b = a;
  int&       c = a;

  decltype(a) x = a;  // int
  static_assert(std::is_same<decltype(x), int>::value, "x should be int");

  decltype(b) y = b;  // const int&
  static_assert(std::is_same<decltype(y), const int&>::value, "y should be int&");

  decltype(c) m = c;  // int&
  static_assert(std::is_same<decltype(m), int&>::value, "m should be int&");

  decltype(a + b) z = a + b;  // int
  static_assert(std::is_same<decltype(z), int>::value, "z should be int");

  std::cout << "All static assertions passed!\n";
}

}  // namespace TestDecltype

namespace TestStdNext {
/*
std::next is a standard library function template introduced by C++11 to get the forward (or backward) offset of an
iterator relative to the current iterator position.It is part of the <iterator> header file. std::next can be used to
make code more concise and readable, especially when dealing with iterator operations that require relative position.
*/
void test() {
  std::vector<int> vec{1, 3, 2, 9, 8, 7};
  auto             it      = vec.begin();
  auto             next_it = std::next(it, 2);
  std::cout << "val: " << *next_it << "\n";  // 2
  std::cout << "Values before sorting: " << vec[0] << " " << vec[1] << " " << vec[2] << " " << vec[3] << " " << vec[4]
            << " " << vec[5] << "\n ";  // 1 3 2 9 8 7
  std::sort(next_it, vec.end());
  std::cout << "Values after sorting: " << vec[0] << " " << vec[1] << " " << vec[2] << " " << vec[3] << " " << vec[4]
            << " " << vec[5] << "\n ";  // 1 3 2 7 8 9

  // Offset (default is 1).Can be positive (move forward) or negative (move backward)
  std::cout << *std::next(vec.end(), -2) << "\n";  // 8
}

}  // namespace TestStdNext

void test() {
  // TestStdNext::test();
  // VirtualInClassDestructor::testAll();
  // TestVirtualInClass::test();
  // TestCplusPlusAttribute::test();
  // TestNullKeyWord::test();
  // TestDecltype::test();
  TestFriendKeyWord::test();
}