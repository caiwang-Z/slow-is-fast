#include <iostream>

template<typename T>
void log(T value) {
	std::cout << value << std::endl;
}

namespace TestDestructorBasic {
	class Base {
	public:
		Base() {
			log("Base constructor called.");
		}

		~Base() {
			log("Base destructor called.");
		}

	};

	class Derived : public Base {
	public:
		Derived() {
			log("Derived constructor called.");
		}
		~Derived() {
			log("Derived destructor called.");
		}

	};

	void test() {
		Base* base = new Base();
		delete base;
		log("---------------------------");
		Derived* derived = new Derived();
		delete derived;

		// output
		/*
			Base constructor called.
			Base destructor called.
			-------------------------- -
			Base constructor called.
			Derived constructor called.
			Derived destructor called.
			Base destructor called.
		*/

	}
}

namespace TestVirtualDestructor {
	class Base {
	public:
		Base() {
			_arr = new int[50];
			log("Base constructor called.");
		}

		~Base() {
			delete[] _arr;
			log("Base destructor called.");
		}
		// virtual destructor
		//virtual ~Base() {
		//  delete[] _arr;
		//	log("Base destructor called.");
		//}

		// Tip
		/*
		Whenever you writing a class that it will be extending or that might
		be subclass whenever you are bascially permitting a class to be subclassed.
		You need 100% declare your destructor as virtual
		*/
	private:
		int* _arr;

	};

	class Derived : public Base {
	public:
		Derived() {
			log("Derived constructor called.");
		}
		~Derived() {
			log("Derived destructor called.");
		}

	};

	void test() {
		Base* base = new Base();
		delete base;
		log("---------------------------");
		Base* poly = new Derived();
		delete poly;

		// output without virtual destructor
		/*
			Base constructor called.
			Base destructor called.
			---------------------------
			Base constructor called.
			Derived constructor called.
			Base destructor called.
		*/


		// output with virtual destructor
		/*
			Base constructor called.
			Base destructor called.
			---------------------------
			Base constructor called.
			Derived constructor called.
			Derived destructor called.
			Base destructor called.
		*/
	}
}

void test() {
	TestVirtualDestructor::test();
}