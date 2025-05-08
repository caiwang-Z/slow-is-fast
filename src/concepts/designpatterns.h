#pragma once
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
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
  float       _price;

  public:
  virtual void prepareParts()  = 0;
  virtual void combineParts()  = 0;
  virtual void assembleParts() = 0;
  virtual void applyLabel()    = 0;
  virtual void showProduct()   = 0;
};

class Car : public Toy {
  public:
  void prepareParts() override { std::cout << "Preparing car parts\n"; }

  void combineParts() override { std::cout << "Combining car parts\n"; }

  void assembleParts() override { std::cout << "Assembling car parts\n"; }

  void applyLabel() override { std::cout << "Applying car label\n"; }

  void showProduct() override { std::cout << "Showing car product\n"; }
};

class Bike : public Toy {
  public:
  void prepareParts() { std::cout << "Preparing Bike parts\n"; }

  void combineParts() { std::cout << "Combining Bike parts\n"; }

  void assembleParts() { std::cout << "Assembling Bike parts\n"; }

  void applyLabel() { std::cout << "Applying Bike label\n"; }

  void showProduct() { std::cout << "Showing Bike product\n"; }
};

class Plane : public Toy {
  public:
  void prepareParts() { std::cout << "Preparing Plane parts\n"; }

  void combineParts() { std::cout << "Combining Plane parts\n"; }

  void assembleParts() { std::cout << "Assembling Plane parts\n"; }

  void applyLabel() { std::cout << "Applying Plane label\n"; }

  void showProduct() { std::cout << "Showing Plane product\n"; }
};

}  // namespace FactoryParts

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
}  // namespace ToyFactoryNS

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

}  // namespace FDPClient
}  // namespace TestFactoryDesignPattern

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
namespace CarNS {
class Tire {
  private:
  std::string _name;
  int         _size;  // zoll
  std::string _mark;

  public:
  std::string getName() { return _name; };
  int         getSize() { return _size; };
  std::string getMark() { return _mark; };
  Tire(const std::string& name, const int size, const std::string& mark) : _name(name), _size(size), _mark(mark){};
};

class CheapTire : public Tire {
  public:
  CheapTire() : Tire("Cheap Tire", 18, "good year") {}
};

class LuxuryTire : public Tire {
  public:
  LuxuryTire() : Tire("Luxury Tire", 20, "michelin") {}
};

class Body {
  private:
  std::string _name;
  std::string _material;

  public:
  std::string getName() { return _name; };
  std::string getMaterial() { return _material; };
  Body(const std::string& name, const std::string& material) : _name(name), _material(material){};
};
class CheapBody : public Body {
  public:
  CheapBody() : Body("Cheap body", "steel") {}
};

class LuxuryBody : public Body {
  public:
  // LuxuryBody() { Body("Luxury body", "carbon"); } // Compile error: In C++, the base class constructors must be
  // explicitly called in the member initializer list of the derived class constructors.

  LuxuryBody() : Body("Luxury body", "carbon") {}
};

class Car {
  private:
  std::string _name;
  Tire*       _tire;
  Body*       _body;

  public:
  Car(const std::string& name) : _name(name) {}
  ~Car() {
    delete _tire;
    delete _body;
  }
  void setTire(Tire* tire) { _tire = tire; }
  void setBody(Body* body) { _body = body; }
  void printInfo() {
    std::cout << "Car name: " << _name << ", " << _body->getName() << ", " << _body->getMaterial() << ", "
              << _tire->getName() << ", brand: " << _tire->getMark() << ", " << _tire->getSize() << " zoll\n";
  }
};

}  // namespace CarNS

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

}  // namespace CarFactoryNS

namespace AFDPClient {
using CarNS::Car;
using namespace CarFactoryNS;
void test() {
// #define CREATE_LUXURY_CAR
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

}  // namespace AFDPClient

}  // namespace TestAbstractFactoryDesignPattern

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
  GameSetting(const GameSetting& setting)             = delete;
  GameSetting& operator=(const GameSetting& settting) = delete;

  private:
  int _width;
  int _height;
  int _brightness;
  GameSetting(int width, int height, int brightness) : _width(width), _height(height), _brightness(brightness){};
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

  // GameSetting set1(*setting); // compile error, a deleted function
  //  GameSetting set2 = *setting;  // compile error, a deleted funciton

  delete setting;
}

}  // namespace TestSingletonDesignPattern

namespace TestSingletonThreadSafety {
class GameSettingTS {
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
  GameSettingTS(const GameSettingTS& setting)             = delete;
  GameSettingTS& operator=(const GameSettingTS& settting) = delete;

  private:
  int _width;
  int _height;
  int _brightness;
  GameSettingTS(int width, int height, int brightness) : _width(width), _height(height), _brightness(brightness){};
  static GameSettingTS* _instance;
  static std::mutex     _mutex;
};

}  // namespace TestSingletonThreadSafety

namespace TestObserverDesignPattern {
/*
@Definition
Defines a one to many dependency between objects so that when one object changes state, all its dependents are notified
and updated automatically.
*/
class Observer;
class Car {
  private:
  int                    _position;
  std::vector<Observer*> _observers;

  public:
  Car(){};
  int  getPosition() { return _position; }
  void setPosition(const int position) {
    _position = position;
    notify();
  }

  void attach(Observer* observer) { _observers.push_back(observer); }

  void detach(Observer* observer) {
    _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
  }

  void notify();
};

class Observer {
  private:
  Car* _car;

  public:
  Observer(Car* car) : _car(car) { _car->attach(this); };
  virtual void update() = 0;
  Car*         getCar() { return _car; }
};

void Car::notify() {
  std::for_each(_observers.begin(), _observers.end(), [](Observer* ob) { ob->update(); });
};

// -1 left lane, 0 middle lane, 1 right lane
class LeftLaneObserver : public Observer {
  public:
  LeftLaneObserver(Car* car) : Observer(car) {}
  void update() override {
    if (getCar()->getPosition() == -1) {
      std::cout << "[Info - left lance observer] Runing in left lane\n";
    }
  }
};

class MiddleLaneObserver : public Observer {
  public:
  MiddleLaneObserver(Car* car) : Observer(car) {}

  void update() override {
    if (getCar()->getPosition() == 0) {
      std::cout << "[Info - middle lane observer] Runing in middle lane \n";
    }
  }
};

class RightLaneObserver : public Observer {
  public:
  RightLaneObserver(Car* car) : Observer(car) {}

  void update() override {
    if (getCar()->getPosition() == 1) {
      std::cout << "[Info - right lane observer] Runing in right lane \n";
    }
  }
};

void test() {
  Car*                car      = new Car();
  LeftLaneObserver*   leftOb   = new LeftLaneObserver(car);
  MiddleLaneObserver* middleOb = new MiddleLaneObserver(car);
  RightLaneObserver*  rightOb  = new RightLaneObserver(car);

  std::cout << "[Command] l: left, m: middle, r: right, b: program exit\n";
  char button;
  bool programmExit = false;
  while (!programmExit) {
    std::cin >> button;
    switch (button) {
      case 'l':
        car->setPosition(-1);
        break;
      case 'm':
        car->setPosition(0);
        break;
      case 'r':
        car->setPosition(1);
        break;
      case 'b':
        programmExit = true;
        break;
      default:
        std::cout << "Invalid parameter\n";
        break;
    }
  }
  delete car;
  delete leftOb;
  delete middleOb;
  delete rightOb;
}

}  // namespace TestObserverDesignPattern

namespace TestPrototypeDesignPattern {
/*
@Intent
1. Creating an object is expensive operation than copy an object.
2. All objects Initial State is common and takes time to build.
3. Composition, creation and representation of objects should be decoupled from the system.
4. Hide the complexity of creating new instance from the user
5. Which classes to create are specified at runtime.

@Downsides
1. Copy!!! [shallow/deep]
2. Copying an object can be complicated.
3. Circular references class can not be cloned.
4. Over use of this might affect the performance.
*/
class Bullet {
  public:
  Bullet(){};
  Bullet(const std::string& bulletName, double speed, double firePower, double damagePower)
      : _bulletName(bulletName), _speed(speed), _firePower(firePower), _damagePower(damagePower) {}

  ~Bullet(){};
  virtual std::unique_ptr<Bullet> clone() = 0;
  void                            fire(double direction) {
    _direction = direction;
    std::cout << "Name:	" << _bulletName << "\n"
              << "Speed: " << _speed << "\n"
              << "Fire power: " << _firePower << "\n"
              << "Damage power: " << _damagePower << "\n"
              << "direction: " << _direction << "\n";
  }

  private:
  std::string _bulletName;
  double      _speed;
  double      _firePower;
  double      _damagePower;
  double      _direction;
};

class SimpleBullet : public Bullet {
  public:
  SimpleBullet(const std::string& bulletName, double speed, double firePower, double damagePower)
      : Bullet(bulletName, speed, firePower, damagePower){};
  std::unique_ptr<Bullet> clone() { return std::make_unique<SimpleBullet>(*this); }
};

class GoodBullet : public Bullet {
  public:
  GoodBullet(const std::string& bulletName, double speed, double firePower, double damagePower)
      : Bullet(bulletName, speed, firePower, damagePower){};
  std::unique_ptr<Bullet> clone() { return std::make_unique<GoodBullet>(*this); }
};

enum class BulletType { SIMPLE, GOOD };
class BulletFactory {
  public:
  BulletFactory() {
    _bullets[BulletType::SIMPLE] = std::make_unique<SimpleBullet>("Simple bullet", 50, 60, 70);
    _bullets[BulletType::GOOD]   = std::make_unique<SimpleBullet>("Good bullet", 100, 100, 100);
  }

  std::unique_ptr<Bullet> createBullet(BulletType type) { return _bullets[type]->clone(); }

  private:
  std::unordered_map<BulletType, std::unique_ptr<Bullet>> _bullets;
};

void test() {
  BulletFactory factory;
  auto          bullet = factory.createBullet(BulletType::SIMPLE);
  bullet->fire(45);

  bullet = factory.createBullet(BulletType::GOOD);
  bullet->fire(90);
}

}  // namespace TestPrototypeDesignPattern

namespace TestBuilderDesignPattern {
/*
@Why build design pattern
The builder pattern is a design pattern designed to provide a flexible solution to various object creation problems in
object-oriented programming. The intent of the builder design pattern is to separate the construction of a complex
object from its representation.

@overview
The Builder design pattern describes how to solve such problems:

Encapsulate creating and assembling the parts of a complex object in a separate Builder object.
A class delegates object creation to a Builder object instead of creating the objects directly.
A class (the same construction process) can delegate to different Builder objects to create different representations of
a complex object.
*/
// end product
class Plane {
  public:
  Plane(const std::string& type) : _type(type) {}
  void setEngine(const std::string& engine) { _engine = engine; }
  void setBody(const std::string& body) { _body = body; }
  void show() { std::cout << "Plane type: " << _type << "\n Engine: " << _engine << "\n Body: " << _body << "\n"; }

  private:
  std::string _type;
  std::string _engine;
  std::string _body;
};

// PlaneBuilder Abstract Class
// Means all builders should have atleast these methods
class PlaneBuilder {
  public:
  virtual void getPartsDone() = 0;
  virtual void buildEngine()  = 0;
  virtual void buildBody()    = 0;
  Plane*       getPlane() { return _plane; }

  protected:
  Plane* _plane;
};

// PlaneBuilder concrete class
// knows only how to build Propeller Plane
class PropellerBuilder : public PlaneBuilder {
  public:
  void getPartsDone() override { _plane = new Plane("Propeller plane"); }
  void buildEngine() override { _plane->setEngine("Propeller engine"); }
  void buildBody() override { _plane->setBody("Propeller body"); }
};

// PlaneBuilder concrete class
// Knows only how to build Jet Plane
class JetBuilder : public PlaneBuilder {
  public:
  void getPartsDone() override { _plane = new Plane("Jet plane"); }
  void buildEngine() override { _plane->setEngine("Jet engine"); }
  void buildBody() override { _plane->setBody("Jet body"); }
};

// Defines steps and tells to the builder that build in given order.
class Director {
  public:
  Plane* createPlane(PlaneBuilder* builder) {
    builder->getPartsDone();
    builder->buildBody();
    builder->buildEngine();
    return builder->getPlane();
  }
};

void test() {
  Director         dir;
  PropellerBuilder pBuilder;
  JetBuilder       jBuilder;

  Plane* pPlane = dir.createPlane(&pBuilder);
  Plane* jPlane = dir.createPlane(&jBuilder);

  pPlane->show();
  std::cout << "****************split line********\n";
  jPlane->show();

  delete pPlane;
  delete jPlane;
}

}  // namespace TestBuilderDesignPattern

namespace TestAdapterDesignPattern {
/*
Adapter Pattern (Adapter Pattern) is a structural design pattern that allows you to convert the
interface of one class to another interface so that these classes that would not otherwise work together due to
interface incompatibility can work together.

Below is a C++ example that uses the Adapter Pattern. In this example, we will use the adapter
pattern to adapt an existing class to a new interface.

*/
class OldAudioPlayer {
  public:
  void play(const std::string& filename) { std::cout << "Playing MP3 file: " << filename << "\n"; }
};

class AudioPlayer {
  public:
  virtual void play(const std::string& audioType, const std::string& filename) = 0;
};

class AudioPlayerAdapter : public AudioPlayer {
  public:
  AudioPlayerAdapter(OldAudioPlayer* oldplayer) : _oldPlayer(oldplayer) {}

  void play(const std::string& audioType, const std::string& filename) {
    if (audioType == "mp3") {
      _oldPlayer->play(filename);
    } else {
      std::cout << "Unsupported audio type: " << audioType << ", filename: " << filename << "\n";
    }
  }

  private:
  OldAudioPlayer* _oldPlayer;
};

void test() {
  OldAudioPlayer* oldPlayer = new OldAudioPlayer();
  AudioPlayer*    player    = new AudioPlayerAdapter(oldPlayer);

  player->play("mp3", "song.mp3");
  player->play("wav", "song.wav");

  delete player;
  delete oldPlayer;
}

}  // namespace TestAdapterDesignPattern

void test() {
  // TestFactoryDesignPattern::FDPClient::test();
  // TestAbstractFactoryDesignPattern::AFDPClient::test();
  // TestSingletonDesignPattern::test();
  // TestObserverDesignPattern::test();
  // TestPrototypeDesignPattern::test();
  // TestBuilderDesignPattern::test();
  TestAdapterDesignPattern::test();
}