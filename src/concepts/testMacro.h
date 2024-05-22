#pragma once
#include <iostream>

namespace TestMacro {
	// 1. defining func with macro
	/* with backsplash \ at the end of the line without space 
	allows you to type multiplines for one macro definition
#define MAIN int main()\
{\
std::cin.get();\
	
}
*/

	// 2. define the macro
#define PR_DEBUG 1

	// 3. use the macro in a conditional compilation check
#if PR_DEBUG == 1
#define LEVEL 2
#endif
}

namespace TestCPPPredefinedMacros {
#if __cplusplus == 202002L
	std::cout << "C++20" << std::endl;

#endif
	void test() {
		std::cout << "Current file: " << __FILE__ << std::endl;
		std::cout << "Current line: " << __LINE__ << std::endl;
		std::cout << "Current func: " << __func__ << std::endl;
	}
}


void test() {
#if LEVEL == 1
	int a = 1;
#elif LEVEL == 2
	int a = 2;
#endif
	TestCPPPredefinedMacros::test();

}

