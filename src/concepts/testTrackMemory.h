#include <iostream>
#include <memory>

struct AllocationMetrics {
  uint32_t totalAllocated = 0;
  uint32_t totalFreed     = 0;
  uint32_t currentUsage() { return totalAllocated - totalFreed; }
};
static AllocationMetrics s_metrics;

static void printMemoryUsage() {
  std::cout << "Memory usage: " << s_metrics.currentUsage() << " bytes\n";
  return;
}

// non-member operator new or operator delete functions cannot be declared static or
// within a namespace other than the global namespace.
void* operator new(size_t size) {
  s_metrics.totalAllocated += size;
  return malloc(size);
}

void operator delete(void* memory, size_t size) {
  s_metrics.totalFreed += size;
  free(memory);
}

namespace TestTrackMemoryAllocation {

class Object {
  int x, y, z;
};
void test() {
  printMemoryUsage();
  {
    printMemoryUsage();
    std::unique_ptr<Object> objPtr = std::make_unique<Object>();
    printMemoryUsage();
  }
  printMemoryUsage();
  std::string name = "Yan Chernok string";
  printMemoryUsage();
}

}  // namespace TestTrackMemoryAllocation

void test() {
  TestTrackMemoryAllocation::test();
}