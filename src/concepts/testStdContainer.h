#pragma once
#include <array>
#include <deque>
#include <iostream>
#include <map>

#include <string>
#include <vector>

namespace TestStaticArray {
void test() {
  std::array<int, 5> data{1, 2, 3, 4, 5};
  const auto         size = data.size();

  data[0] = 1;
}
}  // namespace TestStaticArray

// test for static array
namespace TestStaticArrayData {
struct imageInfo {
  int width;
  int height;
  int linePitch;
};

void test() {
  std::array<imageInfo, 3> infos{{{100, 200, 300}, {99, 299, 399}, {188, 288, 388}}};
  imageInfo**              info = new imageInfo*;
  *info                         = infos.data();
  std::cout << ((*info) + 1)->linePitch << " " << ((*info) + 2)->height << std::endl;
}
}  // namespace TestStaticArrayData

namespace TestStaticAndDynamicArrayBasic {
void test() {
  int size = 5;
  // int stackArray[size]; // Error! allocate array in stack, size must be known in compile time

  int* heapArray = new int[size];  // Ok! Size of array in heap must not be known in compile time
  delete[] heapArray;
}

}  // namespace TestStaticAndDynamicArrayBasic

namespace TestMyIterator {
template <typename VectorT>
class VectorIterator {
  public:
  /*
  When accessing a type member (like a typedef or a type alias) from a dependent template type (VectorT in this case),
  you must prefix it with typename to inform the compiler that valueType is a type defined within VectorT. This is
  necessary because VectorT is a dependent name (its exact type depends on the template instantiation), and C++
  compilers need typename to parse the template correctly.*/
  using valueType     = typename VectorT::valueType;
  using pointerType   = valueType*;
  using referenceType = valueType&;

  public:
  VectorIterator(pointerType ptr) : _ptr(ptr){};

  VectorIterator& operator++() {  // pre increment
    /*
    This version is generally more efficient because it doesn't need to create a temporary object
    before performing the increment.
    */
    _ptr++;
    // Returning *this allows the increment operation to be used directly within other expressions.
    // For example, using iterators in a loop condition:
    return *this;
  }

  VectorIterator& operator--() {
    _ptr--;
    return *this;
  }

  VectorIterator operator--(int) {
    VectorIterator iterator = *this;
    --(*this);
    return iterator;
  }

  referenceType operator[](int index) { return *(_ptr + index); }

  referenceType operator*() { return *_ptr; }

  pointerType operator->() { return _ptr; }

  bool operator==(const VectorIterator& other) const { return _ptr == other._ptr; }

  bool operator!=(const VectorIterator& other) const { return _ptr != other._ptr; }

  VectorIterator operator++(int) {  // post increment
    /*
    It is defined in the class as operator++(int), where int is a dummy parameter used to
    differentiate it from the prefix version. This operator is less efficient than the prefix version because it
    involves creating a temporary object that holds the state of the iterator prior to the increment.
    */
    VectorIterator iterator = *this;
    ++(*this);
    return iterator;
  }

  private:
  pointerType _ptr;
};
}  // namespace TestMyIterator

namespace TestMyArray {
using TestMyIterator::VectorIterator;

template <typename T, size_t S>
class Array {
  public:
  using valueType = T;
  using Iterator  = VectorIterator<Array<T, S>>;

  public:
  size_t size() const { return S; };

  T& operator[](int index) {
    if (!(index < S)) {
      __debugbreak();
    }
    return _data[index];
  }

  T* data() { return _data; }

  Iterator begin() { return Iterator(_data); }
  Iterator end() { return Iterator(_data + S); }

  private:
  T _data[S];
};

void test() {
  int           size = 5;
  Array<int, 5> arr{};
  arr.size();
  memset(arr.data(), 0, arr.size());
  const auto& arrayReference = arr;
  for (size_t i{0}; i < arrayReference.size(); ++i) {
    arr[i] = 2;
  }

  std::cout << "Iterator based traver of myArray: \n";

  for (Array<int, 5>::Iterator it = arr.begin(); it != arr.end(); ++it) {
    std::cout << *it << " ";
  }
}

}  // namespace TestMyArray

namespace TestMyVector {
using TestMyIterator::VectorIterator;

template <typename T>
class Vector {
  public:
  using valueType = T;
  using Iterator  = VectorIterator<Vector<T>>;

  public:
  Vector() { realloc(2); }

  Iterator begin() { return Iterator(_data); };
  Iterator end() { return Iterator(_data + _capacity); };

  void realloc(const size_t newCapacity) {
    const size_t copySize = (newCapacity >= _capacity ? _capacity : newCapacity);

    T* _newBlock = new T[newCapacity];
    // memcpy(_newBlock, _data, newCapacity);  // for primitiv type it works, but for classes, it won't work, cause
    // constructor won't be called.
    for (size_t i = 0; i < copySize; ++i) {
      _newBlock[i] = _data[i];
    }

    _capacity = newCapacity;
    delete[] _data;
    _data = _newBlock;
  }

  void pushBack(const T& val) {
    if (_fillSize >= _capacity) {
      const auto newSize = _capacity + _capacity / 2;
      realloc(newSize);
      std::cout << "Reallocate a new vector with  " << newSize << " elements. \n";
    }
    _data[_fillSize] = val;
    _fillSize++;
  }

  size_t size() const { return _fillSize; }

  ~Vector() { delete[] _data; }

  T& operator[](size_t index) { return _data[index]; }

  const T& operator[](size_t index) const { return _data[index]; }

  private:
  size_t _capacity{2};  // initialization before calling constructor
  size_t _fillSize{0};
  T*     _data = new T[_capacity];
};

template <typename T>
void print(const Vector<T>& vec) {
  std::cout << "Vector values: ";
  for (size_t i = 0; i < vec.size(); ++i) {
    std::cout << vec[i] << " ";
  }
  std::cout << "\n";
};

void test() {
  Vector<int> vec;
  vec.pushBack(2);
  print(vec);
  vec.pushBack(10);
  print(vec);
  vec.pushBack(99);
  vec.pushBack(100);
  vec.pushBack(7);
  vec.pushBack(87);
  print(vec);
}

void testMyIteratorInt() {
  Vector<int> vec;
  vec.pushBack(2);
  vec.pushBack(10);
  vec.pushBack(99);
  vec.pushBack(100);
  vec.pushBack(7);
  vec.pushBack(87);

  for (Vector<int>::Iterator it = vec.begin(); it != vec.end(); ++it) {
    std::cout << *it << " ";
  }
}

void testMyIteratorString() {
  Vector<std::string> vec;
  vec.pushBack("2");
  vec.pushBack("10");
  vec.pushBack("99");
  vec.pushBack("cherno");
  vec.pushBack("7");
  vec.pushBack("87");

  for (Vector<std::string>::Iterator it = vec.begin(); it != vec.end(); ++it) {
    std::cout << *it << " ";
  }
}

class Vector3 {
  public:
  Vector3(){};

  // scalar constructor
  Vector3(double x, double y, double z) : _x(x), _y(y), _z(z) {}
  Vector3(double scalar) : _x(scalar), _y(scalar), _z(scalar) {}

  // copy constructor
  Vector3(const Vector3& vec) {
    _x = vec._x;
    _y = vec._y;
    _z = vec._z;
  }

  // assignment operator
  Vector3& operator=(const Vector3& vec) {
    _x = vec._x;
    _y = vec._y;
    _z = vec._z;
    return *this;
  }

  private:
  double _x{0.0}, _y{0.0}, _z{0.0};
};

void testAssignmentAndCopyConstructor() {
  // only call scalar constructor, would not call assignment operator.
  Vector3 vec = Vector3(4.0);

  // The assignment operator would be used if the object was already constructed,
  // and then a value was assigned to it later, such as:
  Vector3 vec1;
  vec1 = Vector3(4.0);

  // call copy constructor
  Vector3 vec2(vec1);
}
}  // namespace TestMyVector

namespace TestStdVecotrBasicUsage {
// TODO std::vector::assign usage
}

namespace TestStdDequeUsage {

void test() {
  std::deque<int> dq{1, 2, 8, 9};
  dq.push_front(89);
  dq.push_back(99);

  for (const auto i : dq) {
    std::cout << i << " ";
  }
  std::cout << "\n";

  std::cout << "Last element is: " << dq.back();
  for (const auto i : dq) {
    std::cout << i << " ";
  }
  std::cout << "\n";
  dq.pop_back();
  for (const auto i : dq) {
    std::cout << i << " ";
  }
  std::cout << "\n";
}
}  // namespace TestStdDequeUsage

namespace TestStdTupleBaic {
std::tuple<double, char, std::string> get_student(int id) {
  switch (id) {
    case 0:
      return {3.8, 'A', "Lisa Simpson"};
    case 1:
      return {2.9, 'C', "Milhouse Van Houten"};
    case 2:
      return {1.7, 'D', "Ralph Wiggum"};
    case 3:
      return {0.6, 'F', "Bart Simpson"};
  }

  throw std::invalid_argument("id");
}

void test() {
  const auto student0 = get_student(0);
  std::cout << "ID: 0, "
            << "GPA: " << std::get<0>(student0) << ", "
            << "grade: " << std::get<1>(student0) << ", "
            << "name: " << std::get<2>(student0) << '\n';
}

}  // namespace TestStdTupleBaic

namespace TestStdVector {
void testErase() {
  std::vector<int> list{1, 2, 3, 4};
  list.erase(list.begin(), list.end());
  std::cout << list.size() << std::endl;
}

}  // namespace TestStdVector

namespace TestStdInitializer_list {
/*
When using std::initializer_list, it is suitable for simple initialization and passing constant values, and does not
support modifying elements.
*/

void print(std::initializer_list<int> vars) {
  for (const auto& var : vars) {
    std::cout << var << ", ";
  
  }
  std::cout << "\n";
}

void printNegEff(std::initializer_list<int> vars) {
  for (auto& var : vars) {
    //var += 1;  // Error: expression must be a modified lvalue. 
    std::cout << var << ", ";
  }
  std::cout << "\n";
}

void test() {
  print({1, 2, 3, 4, 5});
}

}

void test() {
  // TestMyVector::test();
  // TestMyVector::testAssignmentAndCopyConstructor();
  // TestMyVector::testMyIteratorString();
  // TestMyArray::test();
  // TestStdDequeUsage::test();
  //TestStdVector::testErase();
  TestStdInitializer_list::test();
}