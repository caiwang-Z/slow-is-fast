#include <iostream>
#include <algorithm>
#include <string>

namespace TestNewFeatures {
	void testStdClamp() {
		const auto min = 42.1;
		const auto max = 56.2;
		const auto val1 = 31;
		const auto val2 = 45;
		const auto val3 = 99;
		const auto res1 = std::clamp(static_cast<double>(val1), min, max); // 31
		const auto res2 = std::clamp(static_cast<double>(val2), min, max); // 45
		const auto res3 = std::clamp(static_cast<double>(val3), min, max); // 99
	}

}

namespace TestTypeTraits {
	template <typename T>
	void printInfo(T value) {
		if constexpr (std::is_integral<T>::value) {
			std::cout << "integer: " << value << std::endl;
		}
		else if constexpr (std::is_floating_point<T>::value) {
			std::cout << "double: " << value << std::endl;
		}
		else {
			std::cout << "others:" << std::endl;
		}
	}

	void testTypeTraitsBasic(){
		printInfo(10);      // 10
		printInfo(3.14);    // 3.14
		printInfo("test");  // test
	}

	// std::true_type and std::false
}

namespace TestStdStringConst {

	int32_t GetUrlInfoInt(const std::string& xmlFieldName, const std::string& xmlData) {
		const std::string field = " " + xmlFieldName + "=";
		std::size_t       pos = xmlData.find(field);
		int32_t           res = 0;
		if (pos != xmlData.npos) {
			pos += field.size();
			std::string verMajorStr;
			while (isalnum(xmlData[++pos])) {
				verMajorStr += xmlData[pos];
			}
			if (!verMajorStr.empty()) {
				res = std::stoi(verMajorStr);
			}
		}
		return res;
	}

}

void test() {

}