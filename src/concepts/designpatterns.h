#pragma once 
#include <string>
#include <iostream>

namespace TestFactoryDesignPattern {
	// Note
	// 1. It creates object for you, rather you initiating object directly.
	// 2. FDP is also known as "Virtual Constructor"
	// Adavantages
	// a. Less code change if we change object creation process
	// b. We create object without exposing creation logic to the client
	// c. We get the benefits of virtual constructor
	namespace FactoryParts {
		class Toy {
		protected:
			std::string _name;
			float _price;
		public:
			virtual void prepareParts() = 0;
			virtual void combineParts() = 0;
			virtual void assembleParts() = 0;
			virtual void applyLabel() = 0;
			virtual void showProduct() = 0;
		};

		class Car : public Toy {
		public:
			void prepareParts() {
				std::cout << "Preparing car parts\n";
			}

			void combineParts() {
				std::cout << "Combining car parts\n";
			}

			void assembleParts() {
				std::cout << "Assembling car parts\n";
			}

			void applyLabel() {
				std::cout << "Applying car label\n";
			}

			void showProduct() {
				std::cout << "Showing car product\n";
			}

		};
	
		class Bike : public Toy {
		public:
			void prepareParts() {
				std::cout << "Preparing Bike parts\n";
			}

			void combineParts() {
				std::cout << "Combining Bike parts\n";
			}

			void assembleParts() {
				std::cout << "Assembling Bike parts\n";
			}

			void applyLabel() {
				std::cout << "Applying Bike label\n";
			}

			void showProduct() {
				std::cout << "Showing Bike product\n";
			}

		};

		class Plane : public Toy {
		public:
			void prepareParts() {
				std::cout << "Preparing Plane parts\n";
			}

			void combineParts() {
				std::cout << "Combining Plane parts\n";
			}

			void assembleParts() {
				std::cout << "Assembling Plane parts\n";
			}

			void applyLabel() {
				std::cout << "Applying Plane label\n";
			}

			void showProduct() {
				std::cout << "Showing Plane product\n";
			}

		};
	
	}

	namespace ToyFactoryNS {
		using namespace FactoryParts;
		class ToyFactory {
		public:
			static Toy* createToy(int type) {
				Toy* toy = nullptr;
				switch (type) {
				case 1:
					toy = new Car();
					break;
				case 2:
					toy = new Bike();
					break;
				case 3:
					toy = new Plane();
					break;
				default:
					std::cout << "Invalid parameter: " << type;
					break;
				}
				toy->prepareParts();
				toy->combineParts();
				toy->assembleParts();
				toy->applyLabel();
				return toy;
			}


		};
	}

	namespace FDPClient {
		using namespace ToyFactoryNS;
		void test() {
			int type = 0;
			while (true) {
				std::cout << "Select object to create: 0. Exit, 1. Car, 2. Bike, 3. Plane\n";
				std::cin >> type;
				if (type == 0) {
					std::cout << "Program exit\n";
					break;
				}
				Toy* toy = ToyFactory::createToy(type);
				if (toy) {
					toy->showProduct();
					delete toy;
				}
			
			}
		
		
		}
	
	}
}


void test() {
	TestFactoryDesignPattern::FDPClient::test();

}