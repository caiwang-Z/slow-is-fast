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

	enum class DebugEntry {
		Id,
		GoldAmount,
		DiamondAmount,
		Timestamp
	};
/*	std::unordered_map<DebugEntry, std::string> debugMap = { 
		{DebugEntry::Id, "ID"},
		{DebugEntry::Timestamp, "Timestamp"},
		{DebugEntry::GoldAmount, "Gold Amount"},
		{DebugEntry::DiamondAmount, "Diamond Amount"}
	};
	*/
	using Timestamp = std::string;
	using ResultSet = std::pair<MiningPayback, Timestamp>;
	using DebugSet = std::pair<std::unordered_map<std::string, int>, Timestamp>;
	using OnProcessFinished = std::function<void(ResultSet&& resultImage)>;
	using OnDebug = std::function<void(const DebugSet& debugset)>;
	constexpr auto prcessingGoldTimeInMilliseconds{ 200 };
	constexpr auto prcessingDiamondTimeInMilliseconds{ 200 };
	
	class ImageProcessingPipeline {
	public:
		ImageProcessingPipeline();
		~ImageProcessingPipeline();
		void setOnProcessFinished(const OnProcessFinished& callback);
		void setOnDebug(const OnDebug& callback);
		void process(MinerData& image);
	private:
		OnProcessFinished _onProcessFinished;
		OnDebug _onDebug;
		
		FeatureImageExtract _extractor;
	};

}