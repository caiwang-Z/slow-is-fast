#include <iostream>
#include <map>
#include <unordered_map>

struct CityRecord {
	std::string name;
	uint64_t population;
	double latitude, longitude;

};

namespace std {
	template<>
	struct hash<CityRecord> {  // make sure custom struct could be hashed
		size_t operator()(const CityRecord& key) {
			return hash<std::string>()(key.name);
		}

	};
}

namespace TestMapBasic {
	// operator= access or insert specified element, Returns a reference to the value that is mapped to a key equivalent to key or x respectively, 
	// performing an insertion if such key does not already exist.
	// at: access specified element with bounds checking
	using std::string;



	std::unordered_map<CityRecord, uint32_t> cityMap;


	void test() {
	}
}

void test() {
	TestMapBasic::test();

}
