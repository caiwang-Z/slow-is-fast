#pragma once

#include <iostream>
#include <array>

namespace TestLocalStaticInFunction {
	void Function() {
		//int i = 0; // output: 1, 1, 1
		static int i = 0; // output: 1, 2, 3
		i++;
		std::cout << i << std::endl;

	}

	void test() {
		Function();
		Function();
		Function();
		
	}

}

namespace TestLocalStaticInSingleton {
	// without static
	class Singleton {
	private:
		static Singleton& get() {
			static Singleton instance;  // extending lifetime of this to be forever
			return instance;
		}

		void hello() {
		
		}


	};

	void test() {
		
	}
}

namespace TestStaticScopeAccessAndLifeTimeInClassFunction {
// TODO static visiblity in function and class, static declaration in class, not in definition
}

void test() {
	TestStaticArrayData::test();
}