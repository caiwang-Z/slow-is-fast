#pragma once
#include <iostream>

// void log(const char* message) {
//	std::cout << message << "\n";
// }

namespace UtilityNameSpace {
template <typename T>
void myLog(T t) {
  std::cout << t << "\n";
}

void splitLine() {
  std::cout << "******************************* split line ********************\n";
}

class Splitter {
  public:
  Splitter(const std::string& func) : _func(func) {
    std::cout << "******************************* " << func << " starts ********************************\n";
  };
  ~Splitter() {
    std::cout << "******************************* " << _func << " ends ********************************\n";
  }

  private:
  std::string _func;
};

#define SIFSplit Splitter splitter(__func__)

}  // namespace UtilityNameSpace
