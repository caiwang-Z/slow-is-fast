#include <iostream>

void PrintName(const std::string& name) {
	std::cout << "[lvalue] " << name << std::endl;
}

void PrintName(std::string&& name) {
	// Temp object, would not exits too long
	std::cout << "[rvalue] " << name << std::endl;
}


void test() {
	std::string firstName = "Yan";
	std::string lastName = "Cherno";

	std::string fullName = firstName + lastName;
	PrintName(fullName);  // lvalue

	PrintName(firstName + lastName); // rvalue
}

/*
	?????????????? ??????
*/