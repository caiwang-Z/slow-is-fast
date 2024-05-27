#pragma once
#include <string>
#include <functional>
#include "DummyImageOperation.h"
#include "FrameBuffer.h"

namespace SIF {
	struct MiningPayback {
		int autoNum;
		int houseNum;
		int goldSaldo;
		int diamondSaldo;
		std::string description;

		bool empty() {
			return autoNum == 0 && houseNum == 0 && goldSaldo == 0 && diamondSaldo == 0;
		}


	};
	using Timestamp = std::string;
	using ResultSet = std::pair<MiningPayback, Timestamp>;
	using OnProcessFinished = std::function<void(ResultSet&& resultImage)>;
	constexpr auto prcessingGoldTimeInMilliseconds{ 200 };
	constexpr auto prcessingDiamondTimeInMilliseconds{ 200 };
	
	class ImageProcessingPipeline {
	public:
		ImageProcessingPipeline();
		~ImageProcessingPipeline();
		void setOnProcessFinished(const OnProcessFinished& callback);
		void process(MinerData& image);
	private:
		OnProcessFinished _onProcessFinished;
		SIF::FeatureImageExtract _extractor;
	};

}