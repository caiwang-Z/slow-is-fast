#include "DummyImageOperation.h"
#include <format>

SIF::FeatureImageExtract::FeatureImageExtract() {};

std::vector<SIF::FeatureImage> SIF::FeatureImageExtract::featureImages(const SIF::DeinterlacedImages& image) {
	std::vector<FeatureImage> res;
	
	FeatureImage goldImage;
	goldImage.type = PropertyType::car;
	auto [id, gold, diamond, timestamp] = image;
	goldImage.num = gold / CarCost;
	goldImage.saldo = gold % CarCost;
	goldImage.description = std::format("ID: {} bought {} cars, saldo: {}.", id, goldImage.num, goldImage.saldo, goldImage.description);
	res.push_back(goldImage);

	FeatureImage diamondImage;
	diamondImage.type = PropertyType::house;
	diamondImage.num = diamond / HouseCost;
	diamondImage.saldo = diamond % HouseCost;
	diamondImage.description = std::format("ID: {} bought {} houses, saldo: {}.", id, diamondImage.num, diamondImage.saldo, diamondImage.description);
	res.push_back(diamondImage);
	return res;
	

}