#include <iostream>
#include <string_view>


namespace MakeStringFaster {
	static uint32_t s_AllocCount = 0;

	void* operator new(size_t size) {
		s_AllocCount++;
		std::cout << "Allocating " << size << " bytes\n";
		return malloc(size);
	}

	void printName(std::string_view name) {
		std::cout << name << std::endl;
		/*
		* No need to printName(const std::string_view name)
		Therefore, adding const to a std::string_view parameter is generally redundant unless you want to enforce that the string_view object itself 
		cannot be redirected within the function, which is a rare requirement.
		*/
		/*
		std::string_view is a preferred choice over std::string for function parameters when you don't need to modify or own the string data. It avoids the overhead of copying the string data, which can lead to performance improvements, especially 
		in functions that are called frequently or with large strings.
		*/
	}

	void test() {
		// In C++, the Small String Optimization (SSO) is a technique used by implementations of std::string to store small strings directly within the string object itself, 
		// avoiding the need for dynamic memory allocation. 
		// std::string name = "Ya";  // 0 allocations because of Small String Optimization (SSO)
		std::string name = "Yan Chernikov"; // 1 allocations
#if 1
		std::string firstName = name.substr(0, 3);
		std::string lastName = name.substr(4, 9);  // 3 allocation
#else
		std::string_view firstName{name.c_str(), 3};
		std::string_view lastName{name.c_str() + 4, 9};  // 1 allocation
#endif

		std::cout << s_AllocCount << " allocations" << std::endl;
		
	}


}

void test() {
	MakeStringFaster::test();

}