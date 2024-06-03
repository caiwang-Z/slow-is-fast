#pragma once
#include "ImageProcessingPipeline.h"
#include <chrono>
#include <mutex>
#include "ImageStreamManager.h"
#include <queue>
#include <filesystem>
#include "commondefinitions.h"

namespace SIF {

	using ResultQueue = std::queue<ResultSet>;
	using DebugQueue = std::queue<DebugSet>;
	constexpr auto g_maxResultQueueSize{ 10 };
	constexpr auto g_maxDebugQueueSize{ 10 };
	class Core {
	public:
		Core();
		maErr start();
		maErr stop();
		maErr waitForResult(ResultSet& res, std::chrono::milliseconds timeout);
		maErr waitForDebug(const std::filesystem::path& path, std::chrono::milliseconds timeout);
		
		maErr stopWaiting();
		maErr enableDebug();
		maErr disableDebug();

	private:
		std::unique_ptr<SIF::ImageProcessingPipeline> _imageProcessingPipeline;

		std::mutex _mtxResult, _mtxDebug;
		ResultQueue _resultQueue;
		DebugQueue _debugQueue;
		std::condition_variable _cvResult, _cvDebug;
		std::unique_ptr<ImageStreamManager> _imageStreamManager;
		std::atomic_bool _killSwitch = false;
		std::atomic_bool _debugEnabled = false;
		OnDebug _debugCallback{ nullptr };
	};


}