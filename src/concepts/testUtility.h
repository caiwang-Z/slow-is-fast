#include <iostream>

namespace TestConditionalAndActionBreakpoints {
void test() {
  for (int i{0}; i < 100; ++i) {
    std::cout << i << std::endl;
  }
  int a = 1;
}
}  // namespace TestConditionalAndActionBreakpoints

namespace MyLog {

template <typename T>
void myLog(const T& message) {
  std::cout << message << std::endl;
}
}  // namespace MyLog
