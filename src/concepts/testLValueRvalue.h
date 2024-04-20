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

namespace TestMoveBasic {
	class String {
	public:
		String() = default;
		String(const char* string) {
			std::cout << "Constructed!" << "\n";
			_size = strlen(string);
			_data = new char[_size + 1];
			memcpy(_data, string, _size);
			_data[_size] = '\0';
		}

		String(const String& other) {
			std::cout << "Copied!" << "\n";
			_size = other._size;
			_data = new char[_size + 1];
			memcpy(_data, other._data, _size);
			_data[_size] = '\0';
		}

		String(String&& other) noexcept {
			std::cout << "Moved!" << "\n";
			_size = other._size;
			_data = other._data;

			other._size = 0;
			other._data = nullptr;
		}

		~String() {
			std::cout << "Destructed!" << "\n";
			delete[] _data;
		}

		void print() {
			if (_data) {
				std::cout << _data << "\n";

			}
		}

		String& operator=(String&& other) {
			// prevent assign variale to itself
			if (this != &other) {
				// handle old resources
				delete[] _data;
				
				std::cout << "Moved!" << "\n";
				_size = other._size;
				_data = other._data;

				other._size = 0;
				other._data = nullptr;
			}
			return *this;
		};


	private:
		char* _data;
		int _size;

	};

	class Entity {
	public:
		Entity(const String& string): _name(string) {
		}

		Entity(String&& string) : _name(std::move(string)) {
		}
		/*
		!No Entity(const String&& string) : _name(std::move(string)) {
		}
		This is because the purpose of the move constructor is to modify the original object in order to 
		transfer its resources to the new object. Using const prevents this modification
		*/

	private:
		String _name;

	};

	void test() {
		Entity en{ "cherno" };
	
	}

	void testMoveAssignmentOperator() {
		std::cout << "Before move***************************\n";
		String apple = "Apple";
		std::cout << "apple: ";
		apple.print();

		String dest{""};
		std::cout << "dest: ";
		dest.print();
		dest = std::move(apple);

		std::cout << "After move***************************\n";
		std::cout << "apple: ";
		apple.print();
		std::cout << "dest: ";
		dest.print();
	}

}


void test() {
	TestMoveBasic::testMoveAssignmentOperator();
}