#include <iostream>

template <typename T>
void log(T value) {
  std::cout << value << std::endl;
}

namespace TestPointerCast {
void test1() {
  uint32_t  size          = 5;
  uint32_t* pAverageColum = new uint32_t[size];
  memset(pAverageColum, 0, size * sizeof(uint32_t));
  uint32_t* pAv;

  pAv = pAverageColum;
  for (uint32_t i = 0; i < size; ++i) {
    pAv[i] = 1;
  }

  uint16_t pImg[] = {1, 2, 3, 4, 5};

  unsigned char* pSrc            = (unsigned char*)pImg;
  auto           resR0           = ((unsigned short*)pSrc)[0];  // 1
  auto           resR1           = ((unsigned short*)pSrc)[1];  // 2
  auto           resR2           = ((unsigned short*)pSrc)[2];  // 3
  auto           resR3           = ((unsigned short*)pSrc)[3];  // 4
  int            redChannelRef   = 0;
  int            blueChannelRef  = 2;
  int            greenChannelRef = 1;
  int            bytesPerItem    = 2;

  // before accumulating
  for (int i = 0; i < size; ++i) {
    std::cout << pAv[i] << " ";
  }
  std::cout << std::endl;  // output: 1 1 1 1 1

  *pAv += ((unsigned short*)pSrc)[redChannelRef * bytesPerItem];  // red
  pAv++;
  *pAv += ((unsigned short*)pSrc)[greenChannelRef * bytesPerItem];  // green
  pAv++;
  *pAv += ((unsigned short*)pSrc)[blueChannelRef * bytesPerItem];  // blue
  pAv++;

  // after accumulating
  pAv = pAverageColum;
  for (int i = 0; i < size; ++i) {
    std::cout << pAv[i] << " ";
  }
  std::cout << std::endl;  // output: 2 4 6 1 1
  delete pAverageColum;
}

void test() {
  test1();
}
}  // namespace TestPointerCast

namespace TestStaticCast {
/*
In C++, static_cast is the go-to cast for well-defined, compile-time–checked conversions that do not involve runtime
type checks. You should reach for static_cast whenever you know the conversion is safe and you want the compiler to
enforce type correctness. Here are the most common scenarios:

1. Converting between arithmetic types
Turning one numeric type into another, e.g.
*/
namespace ConvertingBetweenArithmeticTypes {
// Why: Explicitly documents possible narrowing or precision changes.
// Note : Unlike C - style casts, you can’t accidentally cast a pointer to an integer here.int    i = 65;
void test() {
  int    i = 65;
  float  f = static_cast<float>(i);       // int → float
  char   c = static_cast<char>(i);        // int → char (value modulo 256)
  double d = static_cast<double>(3.14f);  // float → double
}
}  // namespace ConvertingBetweenArithmeticTypes

/*
2. Upcasting and safe downcasting within an inheritance hierarchy
Upcast (derived → base)
Implicitly allowed, but static_cast makes it explicit:
*/
namespace Upcast {
class Base {};
class Derived : public Base {};

void test() {
  Derived* pd = new Derived();
  Base*    pb = static_cast<Base*>(pd);
  if (pb) {
    std::cout << "upcasting succeed" << std::endl;  // yes
  }
  delete pb;
  // delete pd; // crash. Deleted twice
}

}  // namespace Upcast

/*
Downcast (base → derived)
Safe only if you’re absolutely sure at compile time (and run time) of the object’s true type:
*/
namespace DownCast {
class Base {};
class Derived : public Base {};
class Dummy {};

void test() {
  Base*    pb = new Derived(); /* actually points to a Derived */
  Derived* pd = static_cast<Derived*>(pb);
  if (pd) {
    std::cout << "OK as long as pb really pointed at a Derived" << std::endl;
  }
  delete pd;
  // delete pb; // crash. Deleted twice

  // Dummy* dummyPtr = static_cast<Dummy*>(pb); // compile error. Invalid type conversion
}
}  // namespace DownCast

/*
3. Converting to and from void*
You can cast object pointers to void* and back (but only to the exact original type):

Why: Useful for low-level APIs that take void* context pointers.
*/
namespace ConvertToAndFromVoidPtr {
void test() {
  int   x      = 42;
  void* ptr    = static_cast<void*>(&x);
  int*  intPtr = static_cast<int*>(ptr);
  std::cout << *intPtr << std::endl;  // 42
}

}  // namespace ConvertToAndFromVoidPtr

/*
4. Converting enum values to integer types (and vice versa)
Why: Makes the intention explicit and checks that you aren’t doing something wild like casting a pointer.
*/
namespace ConvertEnumValuesToIntegerAndViceVersa {
enum class Color : unsigned int { red = 1, green, blue };
void test() {
  unsigned int u = static_cast<unsigned int>(Color::green);  // enum -> int
  Color        c = static_cast<Color>(3);                    // int → enum (dangerous if invalid)
}

}  // namespace ConvertEnumValuesToIntegerAndViceVersa

/*
5. Explicitly disabling implicit conversions
If a conversion is legal but you want to signal “yes, I really want this,” static_cast can make code more
self-documenting, e.g.:
*/
namespace ExplicitlyDisablingImplicitConversion {
class A {
  public:
  explicit A(int a) : _a(a){};

  private:
  int _a;
};

void test() {
  // A a1 = 42; // error: explicit constructor
  A a2 = static_cast<A>(42);  // OK: you opted in
}

}  // namespace ExplicitlyDisablingImplicitConversion

void test() {
  Upcast::test();
  DownCast::test();
  ConvertingBetweenArithmeticTypes::test();
  ConvertEnumValuesToIntegerAndViceVersa::test();
}
}  // namespace TestStaticCast

namespace TestCastBasic {
void test() {
  // C style cast
  double val = 5.25;
  double a   = (int)(val + 5.3);

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
    log("Not type of AnotherClass");  // yes
  }
  Derived* de = dynamic_cast<Derived*>(base);
  if (de) {
    log("Is type of Derived");  // yes
  }
}
}  // namespace TestDynamicCast

void test() {
  TestStaticCast::test();
  // TestDynamicCast::test();
  //  TestPointerCast::test();
}