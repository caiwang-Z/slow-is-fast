#include "ImageProcessingPipeline.h"
#include "SIFLog.h"
#include <format>


SIF::ImageProcessingPipeline::ImageProcessingPipeline() {
	_extractor = FeatureImageExtract();
};
SIF::ImageProcessingPipeline::~ImageProcessingPipeline() {};

void SIF::ImageProcessingPipeline::setOnProcessFinished(const OnProcessFinished& callback) {
	_onProcessFinished = callback;
}

void SIF::ImageProcessingPipeline::setOnDebug(const OnDebug& callback) {
	_onDebug = callback;
}

void SIF::ImageProcessingPipeline::process(MinerData& image) {
	// assert() if image is empty

	Log::info(std::format("Processing image generated on {} starts...", image.timestamp));
	
	// saving debug data if it is enabled
	if (_onDebug) {
		const std::unordered_map<std::string, int> data = {
			{"ID", image.id},
			{"Gold Amount", image.goldAmount},
			{"Diamond Amount", image.diamondAmount}
		};
		_onDebug(std::make_pair(data, image.timestamp));
	}

	// process image
	SIF::DeinterlacedImages deinterImages;
	deinterImages = std::make_tuple(image.id, image.goldAmount, image.diamondAmount, image.timestamp);
	// one car equals 2 golds, one house equals 2 diamonds
	const auto processStart{ std::chrono::steady_clock::now() };
	const auto res = _extractor.featureImages(deinterImages);
	const auto processEnd{ std::chrono::steady_clock::now() };
	const auto duration{ std::chrono::duration_cast<std::chrono::milliseconds>(processEnd - processStart).count()};
	std::this_thread::sleep_for(std::chrono::milliseconds(prcessingGoldTimeInMilliseconds));

	MiningPayback pb;
	pb.autoNum = res[0].num;
	pb.goldSaldo = res[0].saldo;
	pb.houseNum = res[1].num;
	pb.diamondSaldo = res[1].saldo;
	pb.description = res[0].description + " " + res[1].description;
	_onProcessFinished(std::move(ResultSet{pb, image.timestamp}));

	Log::info(std::format("Processing image generated on {} finished! After datamining id {} bought {} cars, {} houses, still has {} golds and {} diamonds.", image.timestamp, image.id, pb.autoNum, pb.houseNum, pb.goldSaldo, pb.diamondSaldo));


}