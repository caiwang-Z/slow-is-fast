#pragma once
#include <iostream>

//void log(const char* message) {
//	std::cout << message << "\n";
//}

namespace UtilityNameSpace {
	template<typename T>
	void myLog(T t) {
		std::cout << t << "\n";
	}

	void splitLine() {
		std::cout << "******************************* split line ********************\n";
	}

}

