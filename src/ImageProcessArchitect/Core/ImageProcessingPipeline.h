#pragma once
#include <string>

namespace SIF {
	struct MiningPayback {
		int autoNum;
		int homeNum;
		int saldo;
		std::string description;


	};
	using Timestamp = std::string;
	using ResultSet = std::pair<MiningPayback, Timestamp>;


}