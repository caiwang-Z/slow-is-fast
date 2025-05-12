#pragma once
#include <vector>
#include <iostream>

namespace TestBubbleSort {
// Standard Bubble Sort: sorts the array in ascending order
void bubbleSort(std::vector<int>& arr) {
  const int n = arr.size();
  // Traverse through all elements
  for (int i = 0; i < n - 1; ++i) {
    // Last i elements are already in correct position
    for (int j = 0; j < n - i - 1; ++j) {
      // Swap if the element found is greater than the next element
      if (arr[j] > arr[j + 1]) {
        std::swap(arr[j], arr[j + 1]);
      }
    }
  }
}

// Optimized Bubble Sort: stops if the array is already sorted
void optimizedBubbleSort(std::vector<int>& arr) {
  const int  n = arr.size();
  bool swapped;
  // Traverse through all elements
  for (int i = 0; i < n - 1; ++i) {
    swapped = false;
    // Last i elements are already in correct position
    for (int j = 0; j < n - i - 1; ++j) {
      // Swap if the element found is greater than the next element
      if (arr[j] > arr[j + 1]) {
        std::swap(arr[j], arr[j + 1]);
        swapped = true;
      }
    }
    // If no two elements were swapped by inner loop, then break
    if (!swapped) {
      break;
    }
  }
}

void test(){
  std::vector<int> arr = {64, 34, 25, 12, 22, 11, 90};

  // Print the array before sorting
  std::cout << "Before sorting: ";
  for (int x : arr) {
    std::cout << x << " ";
  }
  std::cout << std::endl;

  // Call the sorting function
  //bubbleSort(arr);
  // Or use the optimized version:
   optimizedBubbleSort(arr);

  // Print the array after sorting
  std::cout << "After sorting:  ";
  for (int x : arr) {
    std::cout << x << " ";
  }
  std::cout << std::endl;

}

}  // namespace TestBubbleSort

void test() {
  TestBubbleSort::test();
}