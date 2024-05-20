#include <iostream>
#include <optional>
#include <fstream>
#include <variant>


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
			std::string& string = *data; // like smart pointer
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
		auto* value = std::get_if<std::string>(&data);

		data = 2;
		std::cout << std::get<int>(data) << "\n";
		auto* value1 = std::get_if<int>(&data);

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
		uint64_t offset = offsetof(Entity, _name); // 8, the address of nullptr (NULL in c) is 0x00000000000
		std::cout << "name of entity: " << en->printName(); // Exception thrown: read access violation.
		// **this** was 0x8. (check offset of _name in class Entity)
	}


}

namespace TestNodiscard {
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

	
	void test() {
		// sum(10, 20);  // warning C4834: discarding return value of function with 'nodiscard' attribute
		auto res = sum(10, 20); // now warning
		// There is a memory leak produced because the returned value was not stored anywhere and 
		// there is no way to deallocate its memory.
		// getNewMessage();  // warning C4834: discarding return value of function with 'nodiscard' attribute
		auto res = getNewMessage(); // no warning
	}

}

void test() {
	TestNodiscard::test();

	//TestNullKeyWord::test();

}