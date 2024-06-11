#pragma once
#include <iostream>
#include <optional>
#include <fstream>
#include <variant>
#include "utility.h"

using namespace UtilityNameSpace;

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
		auto res1 = getNewMessage(); // no warning
	}

}

namespace TestVirtualInClass{
	class Entity {
	public:
		std::string getName() { return "Entity";
		}

	};

	class Player : public Entity {
	public:
		Player(const std::string& name) : _name(name){}
		std::string getName() {
			return _name;
		}
	private:
		std::string _name;
	};


	void testPlayer(){
		Entity* en = new Entity();
		Entity* pl = new Player("Player");
		Player* pl2 = new Player("Player2");
		std::cout << en->getName() << "\n";  // Entity
		std::cout << pl->getName() << "\n"; // Entity
		std::cout << pl2->getName() << "\n"; // Player2
		delete en;
		delete pl;
		delete pl2;
	}

	namespace NSwithOverride{
		class EntityNew {
		public:
			virtual std::string getName() {
				return "Entity";
			}

		};

		class PlayerNew : public EntityNew {
		public:
			PlayerNew(const std::string& name) : _name(name) {}
			std::string getName() override {
				return _name;
			}
		private:
			std::string _name;
		};


		void testPlayer() {
			EntityNew* en = new EntityNew();
			EntityNew* pl = new PlayerNew("Player");
			PlayerNew* pl2 = new PlayerNew("Player2");
			std::cout << en->getName() << "\n";  // Entity
			std::cout << pl->getName() << "\n"; // Player
			std::cout << pl2->getName() << "\n"; // Player2
			delete en;
			delete pl;
			delete pl2;
		}
	
	}

	void test(){
		testPlayer();
		splitLine();
		NSwithOverride::testPlayer();
	}

}

namespace VirtualInClassDestructor{
	class Entity {
	public:
		Entity(){
			std::cout << "Entity construcor called\n";
		}

		~Entity(){std::cout << "Entity destructor called\n"; }
	};

	class Player : public Entity {
	public:
		Player(const std::string& name) : _name(name) {
			std::cout << _name << " constructor called\n";
		}
		~Player(){
			std::cout << _name << " destructor called\n";
		}
	private:
		std::string _name;
	};

	void test(){
		Entity* en = new Player("Player");
		delete en; // Entity construcor called // Player constructor called // Entity destructor called
	
	}

	class Entity1 {
	public:
		Entity1() {
			std::cout << "Entity1 construcor called\n";
		}

		virtual ~Entity1() { std::cout << "Entity1 destructor called\n"; }
	};

	class Player1 : public Entity1 {
	public:
		Player1(const std::string& name) : _name(name) {
			std::cout << _name << " constructor called\n";
		}
		~Player1() {
			std::cout << _name << " destructor called\n";
		}
	private:
		std::string _name;
	};

	void test1() {
		Entity1* en = new Player1("Player1");
		delete en; // Entity1 construcor called // Player1 constructor called //Player1 destructor called  // Entity1 destructor called

	}

	void testAll(){
		test();
		splitLine();
		test1();
	
	}

}

void test() {
	//TestVirtualInClass::test();
	VirtualInClassDestructor::testAll();
	//TestNodiscard::test();

	//TestNullKeyWord::test();

}