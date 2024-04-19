#include <iostream>

namespace TestLValueRvalueBasic {
	/*
	1. lvalue is a lot of times something that is on the left side of the equal sign
	and then rvlaue is something that is on the right side of equal sign. This point 
	is not always true. Just give a kind of feeling what lvalue and rvalue are.
	2. You can only have a lvalue reference from an rvalue.
	**************************
	void setValue(int value);
	setValue(i); // ok
	setValue(10); // ok
	**************************
	void setValue(int& value);
	setValue(i); // ok
	setValue(10); // error. Initial value of reference to non-const must be an lvalue
*/
	void printName(const std::string& name) {
		// only accpet lvalue
		std::cout << "[lvalue] " << name << "\n";
	}

	void printName(std::string&& name) {
		// only accpet rvalue
		std::cout << "[rvalue] " << name << "\n";

		// main adavantage is to do with optimization. If we know it is a
		// temporary object. Then we donot have to worry about making sure that
		// we keep it alive and we can steal the resources that might be attached to 
		// that specific object and use them somewhere else. But not for lvalue, cause it could
		// be referenced somewhere else.
	}

	void test() {
		std::string firstName = "Yan";
		std::string lastName = "chernikov";

		std::string fullName = firstName + lastName;


	}
}

void test() {

}