#pragma once

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

void test() {
#if LEVEL == 1
	int a = 1;
#elif LEVEL == 2
	int a = 2;
#endif

}