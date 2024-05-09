#include <iostream>
#include <vector>
#include "testUtility.h"
#include <filesystem>

using MyLog::myLog;

namespace TestSTDExceptionBasic {

	void testCatchSTDOutOfRangeException() {
		std::vector<int> vec;
		vec.push_back(1);

		try {
			std::cout << vec.at(1) << std::endl;
			std::cout << "No error \n";
		}
		catch (const std::out_of_range& e) {
			std::cout << "Catch one exception: " << e.what() << "\n";
		}
		catch (...) {
			std::cout << "Catch unknown exceptioin.\n";
		}

		std::cout << "I am still alive.\n";


	
	}

	void testCatchAllException() {
		std::vector<int> vec;
		vec.push_back(1);

		try {
			std::cout << vec.at(1) << std::endl;
			std::cout << "No error \n";
		}
		//catch (const std::out_of_range& e) {
		//	std::cout << "Catch one exception: " << e.what() << "\n";
		//}
		catch (...) {
			std::cout << "Catch unknown exceptioin.\n";
		}

		std::cout << "I am still alive.\n";



	}

}

namespace TestBuiltinThrowException {
	void test() {
		try {
			throw "I am const char excecpiton";
			throw 1;
			throw 1.0f;
			throw 1.1;
			throw 'a';
			throw std::string("Std::string excpetion");
		}
		catch (const int& e) {
			std::cout << "Int exception: " << e << std::endl;
		}
		catch (const float& e) {
			std::cout << "Float exception: " << e << std::endl;
		}
		catch (const double& e) {
			std::cout << "Double exception: " << e << std::endl;
		}
		catch (const char* e) {
			std::cout << "Char exception: " << e << std::endl;
		}
		catch (const std::string& e) {
			std::cout << "Std::string exception: " << e << std::endl;
		}
		catch (...) {
			myLog("Unknown exception.");
		}

	
	}

}

namespace TestUserDefinedException {
	/*
	Benifits of defining custom exceptions
	1. Code readability
	2. Better error handling
	3. Modularity
	4. Readlity
	*/
	class VideoPlayerException : public std::exception {
	public:
		VideoPlayerException(const char* message): _errorMessage(message) {};

		const char* what() const noexcept {
			return _errorMessage;
		}


	private:
		const char* _errorMessage;
	};

	void test() {
		try {
			const std::string fileName = "data.txt";
			using std::filesystem::exists;
			if (!exists(fileName)) {
				throw VideoPlayerException("File does not exist.");
			}
			else {
				// play the video
			}
		}
		catch (const std::exception& e) {
			std::cout << e.what();
		}
		catch (...) {
			std::cout << "Unknown exception.";
		}
	
	}


}

namespace TestRethrowEception {
	class MyException {
	public:
		MyException(const char* message) : _message(message) {};
		const char* what() const noexcept {
			return _message;
		}

	private:
		const char* _message;
	};

	void foo() {
		MyException e("One exception, id: 768");
		
		try {
			throw e;
		}
		catch (const MyException& e) {
			std::cout << "Catch Exception in foo function: " << e.what() << ", but rethrow it" << std::endl;
			throw;
		}
	}

	void testInter() {
		try {
			foo();
		}
		catch (const MyException& e) {
			std::cout << "Catch Exception in test function: " << e.what() << ", but rethrow it" << std::endl;
			throw;
		}
	}

	void test() {
		try {
			testInter();
		}
		catch (const MyException& e) {
			std::cout << "Catch Exception in main function: " << e.what() << ", it is handled." << std::endl;
		}
	}



}

namespace TestStackUnwindingInExceptionHandling {
	/*
		Stack unwinding happens in reverse order and it always happens in case of exception hanling only

	*/
	class A {
	public:
		A() {
			std::cout << "A constructor calls.\n ";
		}

		~A(){
			std::cout << "A destructor calls.\n ";
		}

	};
	class B {
	public:
		B() {
			std::cout << "B constructor calls.\n ";
		}

		~B() {
			std::cout << "B destructor calls.\n ";
		}

	};

	void test() {
		try {
			A a;
			B b; // call destructors in reverse order
			throw("Exception thrown in try block of test function");
		}
		catch (const char* message) {
			std::cout << message << "\n";
		}
		catch (...) {
			std::cout << "Unknown exception.\n";
		}

	
	}


}

namespace TestSetTerminateFunction {
	/*
	*	1. set terminate function, will be called when the program is terminated.
		2. You can set this terminate function for each and every thread separately.
		Because thread will have to handle its exit or terminate or abort independently.
	*/

	void test() {
		std::set_terminate([]() {
			std::cout << "set_terminated exception handled.\n" << std::flush;
			abort();
			});

		terminate();
	}
	
}

void test() {
	TestSetTerminateFunction::test();
	//TestStackUnwindingInExceptionHandling::test();
	//TestRethrowEception::test();
	//TestUserDefinedException::test();
	//TestBuiltinThrowException::test();
	//TestSTDExceptionBasic::testCatchSTDOutOfRangeException();
	//TestSTDExceptionBasic::testCatchAllException();

}