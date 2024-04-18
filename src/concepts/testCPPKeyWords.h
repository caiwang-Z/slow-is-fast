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
		auto* value = std::get_if<int>(&data);

	}

}

void test() {


}