#pragma once
#include "ImageProcessingPipeline.h"
#include <chrono>
#include <mutex>
#include "ImageStreamManager.h"
#include <queue>

namespace SIF {

	using ResultQueue = std::queue<SIF::ResultSet>;
	constexpr auto g_maxQueueSize{ 10 };
	class Core {
	public:
		Core();
		void start();
		void stop();
		void waitForResult(ResultSet& res, std::chrono::milliseconds timeout);
		void stopWaiting();
	private:
		std::unique_ptr<SIF::ImageProcessingPipeline> _imageProcessingPipeline;

		std::mutex _mtxResult;
		ResultQueue _resultQueue;
		std::condition_variable _cvResult;
		std::unique_ptr<ImageStreamManager> _imageStreamManager;
		std::atomic_bool _killSwitch = false;
	};


}