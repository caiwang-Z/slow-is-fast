#include <iostream>
#include <optional>
#include <fstream>


namespace TestOptionalKeyWord {
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

void test() {


}