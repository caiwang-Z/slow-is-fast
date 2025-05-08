#pragma once
#include <vector>

namespace TestReturnMultipleValues {
// 1. std::pair pa; pa.first, pa.second

// 2. std::tuple tu; std::get<0>(tu), std::get<1>(tu)
// both of them are not clear, cause no one knows what means index, 0, 1

// 3. with struct, this one is best
/*
        stuct ShaderProgramSource{
                std::string VertexSource;
                std::string FragmentSource;
        }
*/

// put references or pointers into function, then the ouput variables in parent function needs to be pre allocated.

}  // namespace TestReturnMultipleValues

namespace TestReturnConstReference {
class Container {
  private:
  std::vector<int> data;

  public:
  Container() { data = {1, 2, 3, 4, 5}; }

  const std::vector<int>& getData() const { return data; }
};

void test() {
  Container               container;
  const std::vector<int>& myData = container.getData();
  for (int num : myData) {
    std::cout << num << " ";
  }
  std::cout << "\n";

  // Code below would fail, its not allowed to modify
  // myData.push_back(6);
}
}  // namespace TestReturnConstReference

namespace TestDanglingReference {
int& getNumber() {
  static int localVar = 10;  // without static keyword, then renturn a reference of local variable
  return localVar;           // returning address of local variable or temporary,
}

void test() {
  int& numRef = getNumber();
  numRef      = 99;
  std::cout << numRef << std::endl;
}
}  // namespace TestDanglingReference

void test() {
  // TestDanglingReference::test();
}