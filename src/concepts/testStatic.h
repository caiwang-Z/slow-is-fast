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

// test for static array, move to test array header file

namespace TestStaticArrayData {
	struct imageInfo {
		int width;
		int height;
		int linePitch;
	};

	void test() {
		std::array<imageInfo, 3> infos{{
			{100, 200, 300}, 
			{99, 299, 399}, 
			{188, 288, 388} 
		} };
		imageInfo** info = new imageInfo*;
		*info = infos.data();
		std::cout << ((*info) + 1)->linePitch << " " << ((*info) + 2)->height << std::endl;

	}
}

void test() {
	TestStaticArrayData::test();
}