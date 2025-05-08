#include <chrono>
#include <iostream>
#include <thread>

namespace TestTimingBasic {

void test() {
  using namespace std::literals::chrono_literals;
  auto start = std::chrono::high_resolution_clock::now();
  std::this_thread::sleep_for(1s);
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<float> duration = end - start;
  std::cout << duration.count() << "s " << std::endl;
}
}  // namespace TestTimingBasic

namespace TestScopeTiming {
class Timer {
  private:
  std::chrono::steady_clock::time_point _start, _end;

  public:
  Timer() { _start = std::chrono::high_resolution_clock::now(); }
  ~Timer() {
    _end                                  = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = _end - _start;
    std::cout << "Timer runs " << duration << " s, " << duration * 1000.0f << " ms" << std::endl;
  }
};

void func() {
  Timer timer;

  for (int i = 0; i < 100; ++i) {
    // std::cout << "Hello " << std::endl;  // slow
    std::cout << "Hello \n";  // fast
  }
}

void test() {
  func();
}
}  // namespace TestScopeTiming

// Timer class are used in other files for timing
// void test() {
//	//TestTimingBasic::test();
//	TestScopeTiming::test();
//
//}