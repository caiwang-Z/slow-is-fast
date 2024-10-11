#pragma once

#include <iostream>
#include "calc.h"

void foo() {
  std::cout << "The result of 3 + 5 equals " << CALC::add(3, 5) << std::endl;
}