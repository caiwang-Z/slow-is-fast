#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "utility.h"

using UtilityNameSpace::myLog;
using UtilityNameSpace::splitLine;

namespace TestMySmartPointer {
// TOPIC: Introduction To Smart Pointers

// NOTES:
// 0. Smart pointer is a class which wraps a raw pointer, to manage the life time of the pointer.
// 1. The most fundamental job of smart pointer is to remove the chances of memory leak.
// 2. It makes sure that the object is deleted if it is not reference any more.

// TYPES:
// 1. unique_ptr :
//    Allows only one owner of the underlying pointer.

// 2. shared_ptr :
//    Allows multiple owners of the same pointer (Reference count is maintained).

// 3. weak_ptr :
//    It is special type of shared_ptr which doesn't count the reference.

class MyInt {
  public:
  explicit MyInt(int* data) : _data(data){};
  ~MyInt() {
    delete _data;
    std::cout << "raw pointer has been freed.\n";
  };

  int& operator*() { return *_data; }

  private:
  int* _data;
};

void test() {
  int*  intPtr = new int(3);
  MyInt smPtr(intPtr);
  // MyInt smPtr = intPtr;  // without explict in MyInt constructor it will work.

  std::cout << "Value of smart pointer is : " << *smPtr << std::endl;
}

}  // namespace TestMySmartPointer

namespace TestUniquePointerBasic {
// TOPIC: Unique Pointer In C++

// NOTES:
// 0. unique_ptr is a class template.
// 1. unique_ptr is one of the smart pointer provided by c++11 to prevent memory leaks.
// 2. unique_ptr wraps a raw pointer in it, and de-allocates the raw pointer,
//    when unique_ptr object goes out of scope.
// 3. similar to actual pointers we can use -> and * on the object of unique_ptr,
//    because it is overloaded in unique_ptr class.
// 4. When exception comes then also it will de-allocate the memory hence no memory leak.
// 5. Not only object we can create array of objects of unique_ptr.

// OPERATIONS:
// release: returns a pointer to the managed object and releases the ownership
// reset: 	replaces the managed object
// swap: swaps the managed objects
// get: returns a pointer to the managed object
// , get_deleter: returns the deleter that is used for destruction of the managed object

class Dummy {
  public:
  explicit Dummy(int val) : _data(val){};
  ~Dummy() { std::cout << "Current value: " << _data << ", destructor called.\n"; }
  int getValue() { return _data; };

  private:
  int _data;
};

void testConstructUniquePointer() {
  std::unique_ptr<Dummy> intP1 = std::make_unique<Dummy>(100);
  myLog(intP1->getValue());

  std::unique_ptr<Dummy> intP2(new Dummy(99));
  myLog(intP2->getValue());
}

void testUniqueOwership() {
  std::unique_ptr<Dummy> intP1 = std::make_unique<Dummy>(100);
  std::unique_ptr<Dummy> intP3;
  // intP3 = intP1; // compile error

  Dummy*                 dy = new Dummy(999);
  std::unique_ptr<Dummy> intP5(dy);
  std::unique_ptr<Dummy> intP6(dy);
  myLog(intP5->getValue());
  // myLog(intP6->getValue());  // runtime error, destructor of Dummy will be called twice
}

void testGetFunction() {
  std::unique_ptr<Dummy> intP1 = std::make_unique<Dummy>(100);

  Dummy* dy = intP1.get();
  myLog(dy->getValue());
  if (intP1) {
    myLog(intP1->getValue());  // will be hit. 100
  }
}

void testReleaseFunction() {
  std::unique_ptr<Dummy> intP1 = std::make_unique<Dummy>(100);

  Dummy* dy = intP1.release();
  myLog(dy->getValue());
  if (intP1) {
    myLog(intP1->getValue());
  }
  delete dy;  // intP1 becomes nullptr, manually manage the raw pointer
}

void testGetDeleterFunction() {
  std::unique_ptr<Dummy> intP1 = std::make_unique<Dummy>(100);

  Dummy* dy = intP1.release();
  myLog(dy->getValue());
  auto func = intP1.get_deleter();
  func(dy);  // Current value: 100, destructor called.
}

void test() {
  // testConstructUniquePointer();
  // testUniqueOwership();
  // testGetFunction();
  testReleaseFunction();
}
}  // namespace TestUniquePointerBasic

namespace TestSharePointerBasic {
// TOPIC: Shared Pointer In C++

// NOTES:
// 0. shared_ptr is a smart pointer which can share the ownership of object (managed object).
// 1. Several shared_ptr can point to the same object (managed object).
// 2. It keep a reference count to maintain how many shared_ptr are pointing to the same object.
//    and once last shared_ptr goes out of scope then the managed object gets deleted.
// 3. shared_ptr is threads safe and not thread safe. [what is this??]
//    a. control block is thread safe, reference count increment or decrement
//    b. managed object is not
// 4. There are three ways shared_ptr will destroy managed object.
//    a. If the last shared_ptr goes out of scope.
//    b. If you initialize shared_ptr with some other shared_ptr.
//    c. If you reset shared_ptr.
// 5. Reference count doesn't work when we use reference or pointer of shared_ptr.

class Foo {
  public:
  Foo(int x) : _x(x){};
  ~Foo() { std::cout << "Called Destructor.\n"; }
  int getX() { return _x; }

  private:
  int _x;
};

void testReferenceCount() {
  splitLine();
  std::shared_ptr<Foo> sp1 = std::make_shared<Foo>(99);
  myLog(sp1.use_count());  // 1

  std::shared_ptr<Foo> sp2 = sp1;  // count increment by 1
  std::cout << "sp1 count: " << sp1.use_count() << " , sp2 count: " << sp2.use_count()
            << "\n";  // sp1 count: 2 , sp2 count: 2

  std::shared_ptr<Foo>* sp3 = &sp1;  // no increment
  std::cout << "sp1 count: " << sp1.use_count() << " , sp3 count: " << sp3->use_count()
            << "\n";  // sp1 count: 2 , sp2 count: 2

  std::shared_ptr<Foo>& sp4 = sp1;  // no increment
  std::cout << "sp1 count: " << sp1.use_count() << " , sp3 count: " << sp4.use_count()
            << "\n";  // sp1 count: 2 , sp2 count: 2
}

void func(std::shared_ptr<Foo> sp) {
  std::cout << "Current count: " << sp.use_count() << std::endl;
}

void testControBlockThreadSafe() {
  splitLine();
  std::shared_ptr<Foo> sp = std::make_shared<Foo>(100);
  std::jthread         t1(func, sp), t2(func, sp), t3(func, sp);
  // Output of current count could be from 2 to 4.
  // shared_ptr::use_count() is atomic and thread-safe;
  // It returns a "snapshot" of the moment it was called,
  // and subsequent increments and decrements by other threads do not feed back to the value that was read;
  // Therefore, it can only be used for debugging, not for synchronization or logical judgments in multithreaded
  // threads.
}

namespace UsingMakeShared {
/*
Why use std::make_shared
1. Exception safety
2. Performance (fewer allocations)
3. Cleaner, more concise code
*/
class Foo {};

void test() {
  // Verbose:
  // First allocation: new Foo allocates memory for the actual Foo object.
  // Second allocation: shared_ptr construct allocates memory for the control block.
  std::shared_ptr<Foo> f(new Foo());

  // Concise:
  // only one heap allocation is required
  auto f2 = std::make_shared<Foo>();
}

}  // namespace UsingMakeShared

namespace ControlBlockThreadSafe {
/*
In C++11's std::shared_ptr implementation, there are two memory blocks involved around a managed object, but technically
there is only one "control block". The two memory blocks are:

1.Control Block
Purpose: Maintains reference counts, deleters for type erasure, weak reference counts, and (if not make_shared) pointer
or allocator information for managed objects.

Contents:
Strong reference count (use count): How many std::shared_ptr<T> instances currently "own" this object. Only when the
strong reference count drops to 0 does the control block actually call delete to destroy the managed object T. In other
words, the strong reference count manages the object's lifetime. weak count: How many instances of std::weak_ptr<T>
currently point to this same control block. It does not affect the lifetime of the object T, but guarantees that the
control block itself will not be released until all std::weak_ptr are destroyed. deleter/allocator, or their type-erased
storage(storage of pointers to managed objects (in non-make_shared scenarios)

2. Object Block
Purpose: actually holds the instance of the T object you created.
Note: this is not a "control block", it just holds the objects you want to manage.

*/

void test() {
  // Initial shared_ptr, use_count == 1
  auto sp = std::make_shared<int>(42);

  // Launch N threads, each repeatedly copies and immediately destroys a copy of sp
  const int                N = 4, ITERS = 100000;
  std::vector<std::thread> threads;
  threads.reserve(N);

  for (int t = 0; t < N; ++t) {
    threads.emplace_back([sp]() mutable {
      for (int i = 0; i < ITERS; ++i) {
        std::shared_ptr<int> local = sp;  // atomic++ on use_count
                                          // when local goes out of scope, its destructor does atomic-- on use_count
      }
    });
  }

  // Wait for all threads to finish
  for (auto& th : threads)
    th.join();

  // At this point all locals have been destroyed; only the original sp remains
  std::cout << "Final use_count = " << sp.use_count() << std::endl;  // Final use_count = 1
}

}  // namespace ControlBlockThreadSafe

namespace ObjectManagedBySharedPtrNotThreadSafe {
struct Counter {
  int  value = 0;
  void increment() { ++value; }
  int  get() const { return value; }
};

void worker(std::shared_ptr<Counter> sp) {
  for (int i = 0; i < 100000; ++i) {
    sp->increment();  // Here, write concurrently to Counter::value
  }
}

void test() {
  auto        sp = std::make_shared<Counter>();
  std::thread t1(worker, sp);
  std::thread t2(worker, sp);

  t1.join();
  t2.join();

  std::cout << "Final count: " << sp->get();  // The output might be 156263, 130059 which are lower that 200000. Because
                                              // of data race in concurrency.
}

}  // namespace ObjectManagedBySharedPtrNotThreadSafe

namespace ObjectManagedBySharedPtrThreadSafeWithMutex {
struct Counter {
  int        value = 0;
  std::mutex mtx;
  void       increment() {
    std::lock_guard<std::mutex> lk(mtx);
    ++value;
  }
  int get() const { return value; }
};

void worker(std::shared_ptr<Counter> sp) {
  for (int i = 0; i < 100000; ++i) {
    sp->increment();  // Here, write concurrently to Counter::value
  }
}

void test() {
  auto        sp = std::make_shared<Counter>();
  std::thread t1(worker, sp);
  std::thread t2(worker, sp);

  t1.join();
  t2.join();

  std::cout << "Final count: " << sp->get();  // The output must be 200000. Because
                                              // of mutex, the data race in concurrency has been fixed.
}

}  // namespace ObjectManagedBySharedPtrThreadSafeWithMutex

namespace SharedFromThis {
/*
In C++, if you want an object to be able to "get" a std::shared_ptr "from itself" (instead of having the external
manually save its shared_ptr), you can have the class inherit from std::enable_shared_from_this, and call the
shared_from_this() method it provides. and then call the shared_from_this() method it provides.
Suppose you have a scenario like this:

struct Widget {
 void do_something() {
 // Here you want to get a shared_ptr<Widget> to hold itself
 }
};

If the Widget internally needs to pass itself (this) to an asynchronous task or callback function, using this directly
loses the reference counting management (which could eventually lead to a dangling pointers); and if you pass in an
external shared_ptr manually, the interface is messy and unsafe.

shared_from_this was created to allow an object to "safely" generate a new shared_ptr from itself, knowing that it is
already managed by a shared_ptr.

*/

class Worker : public std::enable_shared_from_this<Worker> {
  public:
  void do_work() {
    // Get shared_ptr<Worker> to ensure that the object is not destroyed during an asynchronous task.
    auto self = shared_from_this();

    std::thread([self]() {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::cout << "Work done\n";
    }).detach();
  }

  ~Worker() { std::cout << "Worker destroyed\n"; }
};

void test() {
  {
    std::shared_ptr<Worker> w = std::make_shared<Worker>();
    w->do_work();
    // w When the scope ends, the reference count becomes 1 (there is still one in the thread lambda)
  }

  // The main thread waits two seconds to make sure the asynchronous task completes
  std::this_thread::sleep_for(std::chrono::seconds(2));
  // After the lambda finishes executing, the reference count drops to 0 and the worker is destroyed.

  // The complete output would be:
  // Work done
  // Worker destroyed
}

}  // namespace SharedFromThis

void test() {
  SharedFromThis::test();
  // testReferenceCount();
  // testControBlockThreadSafe();
  // ControlBlockThreadSafe::test();
  // ObjectManagedBySharedPtrNotThreadSafe::test();
  // ObjectManagedBySharedPtrThreadSafeWithMutex::test();
}

}  // namespace TestSharePointerBasic

namespace TestWeakPointerBasic {
// TOPIC: Weak Pointer In C++

// NOTES:
// 0. If we say unique_ptr is for unique ownership and shared_ptr is for shared ownership then
//    weak_ptr is for non-ownership smart pointer.
// 1. It actually reference to an object which is managed by shared_ptr.
// 2. A weak_ptr is created as a copy of shared_ptr.
// 3. We have to convert weak_ptr to shared_ptr in order to use the managed object.
// 4. It is used to remove cyclic dependency between shared_ptr.
using TestSharePointerBasic::Foo;
void testWeakPtrCount() {
  // returns the number of shared_ptr objects that manage the object
  splitLine();
  std::shared_ptr<Foo> sp1 = std::make_shared<Foo>(99);
  std::weak_ptr<Foo>   wp1(sp1);

  std::cout << "Shared ptr count: " << sp1.use_count() << ", Weak ptr count: " << wp1.use_count()
            << "\n";  // Shared ptr count: 1, Weak ptr count: 1
}

void testExpire() {
  // checks whether the referenced object was already deleted
  splitLine();
  std::shared_ptr<Foo> sp1 = std::make_shared<Foo>(99);
  std::weak_ptr<Foo>   wp1(sp1);
  std::cout << "Object was deleted? " << wp1.expired() << "\n";  // false

  sp1.reset();
  std::cout << "Object was deleted? " << wp1.expired() << "\n";  // true
}

void testLock() {
  // creates a shared_ptr that manages the referenced object
  splitLine();
  std::shared_ptr<Foo> sp1 = std::make_shared<Foo>(99);
  std::weak_ptr<Foo>   wp1(sp1);
  if (std::shared_ptr<Foo> sp2 = wp1.lock()) {  // check if created shared_ptr object is valid(nullptr or non nullptr)
    std::cout << "Value: " << sp2->getX() << "\n";                 // 99
    std::cout << "Shared ptr count: " << sp2.use_count() << "\n";  // 2
  }

  myLog("Trying to reset weak pointer\n");
  wp1.reset();  // releases the ownership of the managed object, after releases, the shared_ptr object could not be
                // created.
  if (std::shared_ptr<Foo> sp2 = wp1.lock()) {  // check if created shared_ptr object is valid(nullptr or non nullptr)
    std::cout << "Value: " << sp2->getX() << "\n";                 // would not be hit
    std::cout << "Shared ptr count: " << sp2.use_count() << "\n";  // would not be hit
  }

  std::cout << "Shared ptr count: " << sp1.use_count() << "\n";  // 1
  sp1.reset();
  if (std::shared_ptr<Foo> sp2 = wp1.lock()) {
    std::cout << "Value: " << sp2->getX() << "\n";                 // would not be hit
    std::cout << "Shared ptr count: " << sp2.use_count() << "\n";  // would not be hit
  } else {
    myLog("Do not get the resource, maybe the managed object has been deleted.\n");
  }
}

void testDereferenceWeakPtr() {
  splitLine();
  std::shared_ptr<Foo> sp1 = std::make_shared<Foo>(99);
  std::weak_ptr<Foo>   wp1(sp1);
  // std::cout << "Weak pointer value: " << wp1->getX() << std::endl;  // compile error
}

class DummyClass {
  public:
  DummyClass(int x) : _x(x){};
  ~DummyClass() { std::cout << "Called Destructor.\n"; }
  int getX() const { return _x; }
  int getX100() { return _x + 100; }

  private:
  int _x;
};

void testConst() {
  splitLine();
  std::shared_ptr<DummyClass> sp1 = std::make_shared<DummyClass>(99);
  myLog(sp1->getX());
  myLog(sp1->getX100());

  std::shared_ptr<const DummyClass> sp2 = std::make_shared<DummyClass>(99);
  myLog(sp2->getX());
  // myLog(sp2->getX100());  // compile error, could only call non const member functions.
}

void testAssignSharedPtrToWeakPtr() {
  splitLine();
  std::shared_ptr<Foo> sp1 = std::make_shared<Foo>(99);
  std::weak_ptr<Foo>   wp1 =
    sp1;  // declares a weak pointer that refers to an object managed by sp1, the same like wp1(sp1)
}

void test() {
  testWeakPtrCount();
  testExpire();
  testLock();
  testDereferenceWeakPtr();
}
}  // namespace TestWeakPointerBasic

namespace TestcyclicDependencyBetweenSharedptr {
class Son;
class Mother {
  public:
  Mother(){};
  ~Mother() { myLog("Mother object destructed.\n"); }

  void setSon(std::shared_ptr<Son> s) { _son = s; }

  private:
  std::shared_ptr<Son> _son;
};

class Son {
  public:
  Son(std::shared_ptr<Mother> moPtr) : _mother(moPtr){};
  ~Son() { myLog("Son object destructed.\n"); }

  private:
  std::shared_ptr<const Mother> _mother;
};

void testClyclic() {
  std::shared_ptr<Mother> sp_mother = std::make_shared<Mother>();
  std::shared_ptr<Son>    sp_son    = std::make_shared<Son>(sp_mother);

  sp_mother->setSon(sp_son);
}

void test() {
  testClyclic();
  // Mother and son objects could not be destructed because of cyclic dependencies.
}

}  // namespace TestcyclicDependencyBetweenSharedptr

namespace TestcyclicDependencyBetweenSharedptrSolve {
class Son;
class Mother {
  public:
  Mother(){};
  ~Mother() { myLog("Mother object destructed.\n"); }

  void setSon(const std::shared_ptr<Son> s) { _son = s; }

  private:
  std::weak_ptr<Son> _son;
};

class Son {
  public:
  Son(std::shared_ptr<Mother> moPtr) : _mother(moPtr){};
  ~Son() { myLog("Son object destructed.\n"); }

  private:
  std::shared_ptr<const Mother> _mother;
};

void testClyclic() {
  std::shared_ptr<Mother> sp_mother = std::make_shared<Mother>();
  std::shared_ptr<Son>    sp_son    = std::make_shared<Son>(sp_mother);

  sp_mother->setSon(sp_son);
}

void test() {
  testClyclic();
  // Mother and son objects could be destructed because of solving cyclic dependencies problem by using weak_ptr.
}

}  // namespace TestcyclicDependencyBetweenSharedptrSolve

void test() {
  // TestcyclicDependencyBetweenSharedptrSolve::test();
  // TestcyclicDependencyBetweenSharedptr::test();

  TestWeakPointerBasic::test();
  // TestUniquePointerBasic::test();
  // TestSharePointerBasic::test();
  // TestMySmartPointer::test();
}