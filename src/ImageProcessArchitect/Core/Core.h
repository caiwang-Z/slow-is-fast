#pragma once
#include "ImageProcessingPipeline.h"
#include <chrono>

namespace SIF {



	class Core {
	public:
		Core();
		void start();
		void stop();
		void waitForResult(std::pair<MiningPayback, Timestamp>& res, std::chrono::milliseconds timeout);



	};


}