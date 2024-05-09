#pragma once
#include <iostream>

//void log(const char* message) {
//	std::cout << message << "\n";
//}

namespace LogNameSpace {
	template<typename T>
	void myLog(T t) {
		std::cout << t << "\n";
	}

}

