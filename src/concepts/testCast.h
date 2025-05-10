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

namespace TestDynamicCastComplete {
/*
In C++, dynamic_cast is your tool of choice when you need a safe, runtime-checked conversion within an inheritance
hierarchy. Unlike static_cast, which trusts you to know the true type, dynamic_cast will verify at runtime that the cast
is valid (using RTTI), and will fail gracefully if it isn’t.

1. When to use dynamic_cast
Downcasting in polymorphic hierarchies
When you have a pointer or reference to a base class but need to call a member that only exists on a derived class—and
you can’t be 100% sure at compile time that the object really is that derived type.

Cross-casting (sibling casts)
If you have two classes A and B that both derive (directly or indirectly) from the same polymorphic base Base, you can
safely cast between them via Base*:
*/
namespace Basic {
class Base {
  public:
  Base(){};
  virtual ~Base() = default;
};
class A : public Base {
  public:
  void fooA() { std::cout << "fooA" << std::endl; };
};
class B : public Base {
  public:
  B(){};
  void fooB() { std::cout << "fooB" << std::endl; }
};

void test() {
  Base* bptr = new B(); /* actually points to a B */
  // Want to treat it as an A if possible:
  if (auto aptr = dynamic_cast<A*>(bptr)) {
    aptr->fooA();  // no
  } else if (auto aptr = dynamic_cast<B*>(bptr)) {
    aptr->fooB();  // yes
  }
  delete bptr;
}
}  // namespace Basic

namespace Example {
struct Animal {
  virtual ~Animal() = default;
};

struct Cat : Animal {
  void meow() { std::cout << "Meow!\n"; }
};

struct Dog : Animal {
  void bark() { std::cout << "Woof!\n"; }
};

void interact(std::unique_ptr<Animal> pet) {
  if (auto cat = dynamic_cast<Cat*>(pet.get())) {
    cat->meow();  // safe: it was a Cat
  } else if (auto dog = dynamic_cast<Dog*>(pet.get())) {
    dog->bark();  // safe: it was a Dog
  } else {
    std::cout << "Unknown animal.\n";
  }
}

void test() {
  interact(std::make_unique<Cat>());  // prints Meow!
  interact(std::make_unique<Dog>());  // prints Woof!
}

}  // namespace Example

void test() {
  Basic::test();
  Example::test();
}

}  // namespace TestDynamicCastComplete

namespace TestConstCast {
/*
In C++, const_cast is the only cast that can add or remove the const (and/or volatile) qualifier from a pointer or
reference. It performs no run-time checks or conversions— it simply tells the compiler “treat this object as more or
less const-qualified than it really is.” Use it sparingly and only when you know it’s safe.

When to use const_cast
1. Interfacing with legacy APIs
Many older C APIs take non-const pointers (e.g. char*) even when they promise not to modify the data. If you have a
const char* or std::string::c_str(), you can cast away const to call such functions.
*/
namespace InterfacingWithLegacyAPIs {
void legacyFunc(char* buf) {
  std::cout << "It works and buf won`t be modified.\n";
}

void test() {
  const char* message{"Hello world"};
  // legacyFunc(message); // Compile error! argument of type "const char *" is incompatible with parameter of type "char
  // *"
  legacyFunc(const_cast<char*>(message));  // It works and buf won`t be modified.
}

}  // namespace InterfacingWithLegacyAPIs

/*
2. Calling a member function marked const that conceptually mutates internal “mutable” state
If you have a const–qualified member function that nonetheless needs to change some internal cache or bookkeeping field
declared mutable, you might cast away the top-level const of this to call a non-const helper.
*/
namespace CallingnNonConstMemberFunctionInsideConstMemberFunction {
class Image {
  public:
  void loadData() { std::cout << "load data...\n"; };

  /*
    This is a const method that logically shouldn't modify the state of the object - for rendering images.
    However, on the first call, the image is not yet loaded and needs to be loaded first.

    display() is const, so you can't call a non-const loadData() by default.
    We call loadData() by temporarily removing the const const const from this via const_cast.
    Since loadData() only modifies the cache members marked as mutable (_loaded and internal image data), it is still
    semantically "const".
  */
  void display() const {
    // loadData(); // Compile Error!
    if (!_load) {
      const_cast<Image*>(this)->loadData();
    }
    // ... render image ...
  }

  private:
  mutable bool _load = false;
};
}  // namespace CallingnNonConstMemberFunctionInsideConstMemberFunction

/*
3. Reusing code that takes non-const references
Sometimes a library function you want to call is declared to take Foo& but you have a const Foo. If you know it won’t
modify the object’s observable state, you can cast away const.
*/
namespace ReusingCodeThatTakesNonConstRef {
class Foo {};

void printFoo(Foo& f){};

void test() {
  const Foo f_const = Foo();

  // printFoo(f_const);// Compile Error!
  printFoo(const_cast<Foo&>(f_const));  // safe if printFoo doesn’t modify f
}

}  // namespace ReusingCodeThatTakesNonConstRef

/*
4. Removing volatile or both qualifiers
In extremely low-level or embedded code you might need to remove volatile (e.g. for atomic operations) or remove both:
*/
namespace RemovingVolatile {
void test() {
  volatile int x  = 0;
  int&         xr = const_cast<int&>(x);
  xr              = 42;  // still writes through the volatile-qualified object
}
}  // namespace RemovingVolatile

void test() {
  InterfacingWithLegacyAPIs::test();
}

}  // namespace TestConstCast

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
  TestDynamicCastComplete::test();
  // TestStaticCast::test();
  //  TestDynamicCast::test();
  //   TestPointerCast::test();
}