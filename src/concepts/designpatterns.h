#pragma once 
#include <string>
#include <iostream>
#include <mutex>
#include <vector>


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
			void prepareParts() override {
				std::cout << "Preparing car parts\n";
			}

			void combineParts() override {
				std::cout << "Combining car parts\n";
			}

			void assembleParts() override {
				std::cout << "Assembling car parts\n";
			}

			void applyLabel() override {
				std::cout << "Applying car label\n";
			}

			void showProduct() override {
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

namespace TestAbstractFactoryDesignPattern {
	/*
	@AFDP Definition
	AFDP defines an abstract class for creating families of related objects but 
	without specifying their concrete sub-class


	@Why/When to use AFDP
	1. You need system to be independent of how objects are created, composed and represented
	2. Show interface not implementation
	3. System need to be configured with one of the multiple family of objects

	*/
	namespace CarNS{
		class Tire {
		private:
			std::string _name;
			int _size; // zoll
			std::string _mark;
		public:
			std::string getName() { return _name; };
			int getSize() { return _size; };
			std::string getMark() { return _mark; };
			Tire(const std::string& name, const int size, const std::string& mark) : _name(name), _size(size), _mark(mark) {};

		};

		class CheapTire : public Tire {
		public:
			CheapTire(): Tire("Cheap Tire", 18, "good year"){}

		};

		class LuxuryTire : public Tire{
		public:
			LuxuryTire(): Tire("Luxury Tire", 20, "michelin"){}

		};

		class Body {
		private:
			std::string _name;
			std::string _material;
		public:
			std::string getName() { return _name; };
			std::string getMaterial() { return _material; };
			Body(const std::string& name, const std::string& material) : _name(name), _material(material) {};
		};
		class CheapBody : public Body {
		public:
			CheapBody(): Body("Cheap body", "steel") {}

		};

		class LuxuryBody : public Body {
		public:
			// LuxuryBody() { Body("Luxury body", "carbon"); } // Compile error: In C++, the base class constructors must be explicitly called in the member 
			// initializer list of the derived class constructors. 
			
			LuxuryBody(): Body("Luxury body", "carbon"){}

		};

		class Car {
		private:
			std::string _name;
			Tire* _tire;
			Body* _body;

		public:
			Car(const std::string& name) : _name(name) {}
			~Car() {
				delete _tire;
				delete _body;
			}
			void setTire(Tire* tire) { _tire = tire; }
			void setBody(Body* body) { _body = body; }
			void printInfo() {
				std::cout << "Car name: " << _name << ", " << _body->getName() << ", " << _body->getMaterial() << ", " << _tire->getName() << ", brand: " << _tire->getMark() << ", " << _tire->getSize() << " zoll\n";

			}

		};
		
	
	}

	namespace CarFactoryNS {
		using namespace CarNS;
		class CarFactory {
		public:
			virtual Car* buildWholeCar() = 0;
		protected:
			virtual Tire* buildTire() = 0;
			virtual Body* buildBody() = 0;

		};

		class CompactCarFactory : public CarFactory {
		public:
			Car* buildWholeCar() override {
				Car* newCar = new Car("Compact car");
				newCar->setTire(buildTire());
				newCar->setBody(buildBody());
				return newCar;
			};
		private:
			Tire* buildTire() override { return new CheapTire(); };
			Body* buildBody() override { return new CheapBody(); };

		};

		class LuxuryCarFactory : public CarFactory {
		public:
			Car* buildWholeCar() override {
				Car* newCar = new Car("Luxury car");
				newCar->setBody(buildBody());
				newCar->setTire(buildTire());
				return newCar;
			}
		private:
			Tire* buildTire() override { return new LuxuryTire(); }
			Body* buildBody() override { return new LuxuryBody(); }

		};
	
	}

	namespace AFDPClient {
		using CarNS::Car;
		using namespace CarFactoryNS;
		void test() {
//#define CREATE_LUXURY_CAR
#ifdef CREATE_LUXURY_CAR
			CarFactory* factory = new LuxuryCarFactory();
#else
			CarFactory* factory = new CompactCarFactory();
#endif
			Car* car = factory->buildWholeCar();
			car->printInfo();

			delete car;
			delete factory;
		}
	
	}

}


namespace TestSingletonDesignPattern {
/*
@Definition
Define a class that has only one instance and provides a global point of access that instance

@Requirements
1. One and only one instance
2. Global access 
3. No ownership
4. Lazy initialization

@Advantage
1. save memory: only one is required so why to create so many
2. single access point: logger, database connection
3. Flexibility: change anytime you want to

@Usage
1. Multithreaded: threadpool
2. Database application: logging
3. Configuration Settings: game setting, application setting, etc.

@how to create
To create a singleton class we must have:
	-- static member
	-- private constructor
	-- static function

*/

	class GameSetting {
	public:
		static GameSetting* getInstance() {
			if (!_instance) {
				_instance = new GameSetting(1080, 912, 43);
			}
			return _instance;
		}

		void printGameSetting() {
			std::cout << "Width: " << _width << "\n"
				<< "Height: " << _height << "\n"
				<< "Brightness: " << _brightness << "\n";
		}

		void setWidth(int width) { _width = width; }
		void setHeight(int height) { _height = height; }
		void setBrightness(int brightness) { _brightness = brightness; }
		GameSetting(const GameSetting& setting) = delete;
		GameSetting& operator=(const GameSetting& settting) = delete;
		

	private:
		int _width;
		int _height;
		int _brightness;
		GameSetting(int width, int height, int brightness) : _width(width), _height(height), _brightness(brightness) {};
		static GameSetting* _instance;


	};

	// If we are declaring any member as a static then we need to define it outside that class
	GameSetting* GameSetting::_instance = nullptr;

	void test() {
		GameSetting* setting = GameSetting::getInstance();
		setting->printGameSetting();
		setting->setBrightness(100);
		setting->setWidth(999);
		setting->printGameSetting();

		//GameSetting set1(*setting); // compile error, a deleted function
		// GameSetting set2 = *setting;  // compile error, a deleted funciton

		delete setting;
	
	}

}

namespace TestSingletonThreadSafety {
	class GameSettingTS{

	public:
		static GameSettingTS* getInstance() {
			std::unique_lock<std::mutex> lk(_mutex);
			if (!_instance) {
				_instance = new GameSettingTS(1080, 912, 43);
			}
			lk.unlock();
			return _instance;
		}

		void printGameSetting() {
			std::cout << "Width: " << _width << "\n"
				<< "Height: " << _height << "\n"
				<< "Brightness: " << _brightness << "\n";
		}

		void setWidth(int width) { _width = width; }
		void setHeight(int height) { _height = height; }
		void setBrightness(int brightness) { _brightness = brightness; }
		GameSettingTS(const GameSettingTS& setting) = delete;
		GameSettingTS& operator=(const GameSettingTS& settting) = delete;


	private:
		int _width;
		int _height;
		int _brightness;
		GameSettingTS(int width, int height, int brightness) : _width(width), _height(height), _brightness(brightness) {};
		static GameSettingTS* _instance;
		static std::mutex _mutex;



	};

}

namespace TestObserverDesignPattern {
/*
@Definition
Defines a one to many dependency between objects so that when one object changes state, all its dependents are notified and updated automatically.
*/
	class Observer;
	class Car {
	private:
		int _position;
		std::vector<Observer*> _observers;
	public:
		Car() {};
		int getPosition() { return _position; }
		void setPosition(const int position) { 
			_position = position; 
			notify();
		}

		void attach(Observer* observer) {
			_observers.push_back(observer);
		}

		void detach(Observer* observer) {
			
		}
		
		void notify() {};
	
	};

}

void test() {
	//TestFactoryDesignPattern::FDPClient::test();
	//TestAbstractFactoryDesignPattern::AFDPClient::test();
	TestSingletonDesignPattern::test();
}