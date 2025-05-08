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
  TestDynamicCast::test();
  // TestPointerCast::test();
}