#pragma once
#include <iostream>
#include <optional>
#include <fstream>
#include <variant>
#include "utility.h"

using UtilityNameSpace::Splitter;

namespace TestOptionalKeyWord {
	// std::optional to store data which may or may not be present
	std::optional<std::string> ReadFileAsString(const std::string& filepath) {
		std::ifstream stream(filepath);
		if (stream) {
			std::string result;
		}
		return {};
	}

	void test() {
		std::optional<std::string> data = ReadFileAsString("data.txt");
		std::string value = data.value_or("No present");
		std::cout << value << std::endl;

		if (data.has_value()) {
			std::cout << "File read sucessfully.";
		}
		else {
			[[maybe_unused]] std::string& string = *data; // like smart pointer
			std::cout << "File could not be opened.";
		}
	
	}

}

namespace TestVariantKeyWord {
	enum class ErrorCode {
		None = 0,
		NotFound = 1,
		NoAccess = 2
	};

	std::variant<std::string, ErrorCode> ReadFileAsString() {
		return {};
	}

	void test() {
		std::variant<std::string, int> data;
		data = "cherno";
		std::cout << std::get<std::string>(data) << "\n";
		[[maybe_unused]] auto* value = std::get_if<std::string>(&data);

		data = 2;
		std::cout << std::get<int>(data) << "\n";
		[[maybe_unused]]  auto* value1 = std::get_if<int>(&data);

	}

}

namespace TestNullKeyWord {
	class Entity {
	public:
		Entity() = default;

		const std::string& printName() {
			return _name;
		}

		void printType() {
			std::cout << "Entity\n" << std::endl;
		}

	public:
		Entity* _parent;
		std::string _name;

	};

	void test() {
		Entity* en = nullptr;
		std::cout << "Line 1 *****************************************\n";
		en->printType();
		std::cout << "Line 2 *****************************************\n";
		[[maybe_unused]] uint64_t offset = offsetof(Entity, _name); // 8, the address of nullptr (NULL in c) is 0x00000000000
		std::cout << "name of entity: " << en->printName(); // Exception thrown: read access violation.
		// **this** was 0x8. (check offset of _name in class Entity)
	}


}
namespace TestCplusPlusAttribute {
	namespace TestNodiscard {
		// The [[nodiscard]] attribute is used to warn the programmer that the return value of a function should be used. 
		// If the function's return value is ignored, the compiler will generate a warning.

		[[nodiscard]]
		int sum(int a, int b) {
			return a + b;
		}

		[[nodiscard]]
		char* getNewMessage() {
			char* ms = new char[100];
			strcpy(ms, "Hello World.");
			return ms;
		}


		void testAttributeNoDiscard() {
			SIFSplit;
			 // sum(10, 20);  // warning C4834: discarding return value of function with 'nodiscard' attribute
			[[maybe_unused]] auto res = sum(10, 20); // now warning
			// There is a memory leak produced because the returned value was not stored anywhere and 
			// there is no way to deallocate its memory.
			// getNewMessage();  // warning C4834: discarding return value of function with 'nodiscard' attribute
			[[maybe_unused]] auto res1 = getNewMessage(); // no warning
		}

	}

	namespace TestDeprecated {
		// The [[deprecated]] attribute is used to mark entities (e.g. functions, variables, types, etc.) that are not recommended for use. 
		// The compiler generates a warning when using entities marked as [[deprecated]].

		[[deprecated("This function will be removed in the future. Use newFunc instead")]]
		void oldFunc() {};
		
		void newFunc() {};

		void testAttributeDeprecated() {
			SIFSplit;
			// oldFunc();  // warning C4996: 'TestCplusPlusAttribute::TestDeprecated::oldFunc': This function will be removed in the future. Use newFunc instead
			newFunc();
		}
		
	}

	namespace TestFallThrough {
		// This helps avoid unexpected control flow errors and makes the code more readable.
		void process(int value) {
			switch (value) {
			case 0:
				std::cout << "case 0 \n";
				[[fallthrough]];
			case 1:
				std::cout << "case 1 \n";
				[[fallthrough]] ;
			case 2:
				std::cout << "case 2 \n";
				break;
			default:
				std::cout << "Default case \n";
				break;
			}
		
		}


		void testAttributeFallThrough() {
			SIFSplit;
			process(0);  // case 0; case 1; case 2
		
		}
	
	}

	namespace TestLikelyAndUnlikely {
		// These attributes help the compiler optimize by suggesting that a branch is more or less likely to be executed. 
		// These attributes were introduced in C++20.
		void testAttributeLikelyAndUnlikelyInIfElse() {
			SIFSplit;
			int x = 10;


			if (x > 1) [[likely]] {
				std::cout << "x is greater than 1\n";
			}
			else if (x == 1) [[unlikely]] {
				std::cout << "x equals 1 \n";
			}
			else {
				std::cout << "x is smaller that 1 \n";
			
			}
		
		}

		void testAttributeLikelyAndUnlikelyInSwitch() {
			SIFSplit;
			int x = 10;
			switch (x) {
			case 0:
				std::cout << "case 0 \n";
				[[unlikely]];
				break;
			case 5:
				std::cout << "case 5 \n";
				[[unlikely]];
				break;
			case 10:
				std::cout << "case 10 \n";
				[[likely]];
				break;
			default:
				std::cout << "default case \n";
				break;
			}
		
		}
	
	}

	namespace TestNoReturn {
	// The [[noreturn]] attribute is used to mark functions that do not return. 
	// This is typically used for functions that throw an exception or terminate the program.
		[[noreturn]] void fatalError(const char* msg) {
			throw "fatal error";
		}

		void testAttributeNoReturn() {
			SIFSplit;

			try {
				fatalError("Something went wrong");

			}
			catch (const char* msg) {
				std::cout << "Exception caught: " << msg << "\n";
			
			}
		
		}
	}

	namespace TestMaybeUnused {
		void foo([[maybe_unused]]int a, int b, int c, int d) {
#ifdef CONDITION
			std::cout << "a: " << a << ",b: " << b << ", c: " << c << ", d: " << d << "\n";
#else
			std::cout << "b: " << b << ", c: " << c << ", d: " << d << "\n";
#endif
		};

		void testAttributeMaybeUnused() {
			SIFSplit;
			[[maybe_unused]] int a = 10;  // no warning
			foo(1, 2, 3, 4);
		}
		

	}


	void test() {
		TestNodiscard::testAttributeNoDiscard();
		TestDeprecated::testAttributeDeprecated();
		TestFallThrough::testAttributeFallThrough();
		TestLikelyAndUnlikely::testAttributeLikelyAndUnlikelyInIfElse();
		TestLikelyAndUnlikely::testAttributeLikelyAndUnlikelyInSwitch();
		TestNoReturn::testAttributeNoReturn();
		TestMaybeUnused::testAttributeMaybeUnused();
	}

}


void test() {
	TestCplusPlusAttribute::test();
	//TestNullKeyWord::test();

}