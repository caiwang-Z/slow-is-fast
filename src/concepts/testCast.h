#include <iostream>

template <typename T>
void log(T value) {
  std::cout << value << std::endl;
}

namespace TestCastBasic {
void test() {
  // C style cast
  double val = 5.25;
  double a = (int)(val + 5.3);

  // C++ style cast
  double c = static_cast<int>(val + 5.3);

  log(a);
  log(c);
}

// benifits of C++ style cast: english words, can search it in code base
// static cast: see if the convertion actually at all ever possible
// reinterpret cast: reinterpreting this memory as something else
// const cast: removing and adding const, mostly for removing const
// dynamic cast: used for safe downcasting at runtime. atempt the conversion and
// do something if it fails
}  // namespace TestCastBasic

namespace TestDynamicCast {
class Base {
 public:
  Base(){};
  virtual ~Base(){};  // must be marked as virtual
                      /*
                       the dynamic_cast operation requires the base class to be polymorphic.
                       This means that the base class must have at least one virtual function.
                       In typical use cases for dynamic_cast, this is often ensured by having
                       a virtual destructor, as polymorphism is a fundamental concept when
                       dealing with inheritance hierarchies where you need safe type conversions at
                       runtime.
                      */
};
class Derived : public Base {
 public:
  Derived(){};
  ~Derived(){};
};
class AnotherClass : public Base {
 public:
  AnotherClass(){};
  ~AnotherClass(){};
};

void test() {
  Derived* derived = new Derived();

  Base* base = derived;

  AnotherClass* ac = dynamic_cast<AnotherClass*>(base);
  if (!ac) {
    log("Not type of AnotherClass");
  }
  Derived* de = dynamic_cast<Derived*>(base);
  if (de) {
    log("Is type of Derived");
  }
}
}  // namespace TestDynamicCast

void test() {
  TestDynamicCast::test();
}