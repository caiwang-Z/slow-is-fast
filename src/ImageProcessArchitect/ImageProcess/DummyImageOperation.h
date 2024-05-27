#pragma once

#include <string>
#include <vector>

namespace SIF {
	enum PropertyType {
		car,
		house
	};
	struct FeatureImage {
		PropertyType type;
		int num;
		int saldo;
		std::string description;

	};
	constexpr auto CarCost{ 2 };
	constexpr auto HouseCost{ 2 };
	using DeinterlacedImages = std::tuple<int, int, int, std::string>;

	class FeatureImageExtract {
	public:
		// can have some config
		FeatureImageExtract();
		std::vector<FeatureImage> featureImages(const DeinterlacedImages& image);


	};

}