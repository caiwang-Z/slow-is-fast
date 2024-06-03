#include "Core.h"
#include "SIFLog.h"
#include "debugdata.h"

namespace SIF {

	Core::Core() {
		_imageProcessingPipeline = std::make_unique<SIF::ImageProcessingPipeline>();
		_imageProcessingPipeline->setOnProcessFinished({ [this](SIF::ResultSet&& featureImage) {
			std::lock_guard<std::mutex> lk(_mtxResult);
			if (_resultQueue.size() >= g_maxResultQueueSize) {
				Log::info("Result image queue is full!!!");
			}
			else {
				_resultQueue.push(featureImage);
			}
			_cvResult.notify_all();
			} });

		_debugCallback = [this](const DebugSet& debugData) {
			std::lock_guard<std::mutex> lk(_mtxDebug);
			if (_debugQueue.size() >= g_maxDebugQueueSize) {
				Log::warn("Debug queue is full!!!");
			}
			else {
				_debugQueue.push(debugData);
			}
			_cvDebug.notify_all();
		};

		_imageStreamManager = std::make_unique<ImageStreamManager>(std::bind_front(&ImageProcessingPipeline::process, &(*_imageProcessingPipeline)));

	}

	maErr Core::enableDebug() {
		_imageProcessingPipeline->setOnDebug(_debugCallback);
		_debugEnabled = true;
		Log::info("Saving debug data enabled.");
		return maErrOK;
	}

	maErr Core::disableDebug() {
		_imageProcessingPipeline->setOnDebug(nullptr);
		_debugEnabled = false;
		Log::info("Saving debug data disabled.");
		return maErrOK;
	}

	maErr Core::start() {
		_imageStreamManager->start();
		Log::info("Core started");
		return maErrOK;
	}

	maErr Core::stop() {
		Log::info("Core stopping...");
		_imageStreamManager->stop();
		Log::info("Core stopped");
		return maErrOK;
	
	}

	maErr Core::waitForResult(ResultSet& res, std::chrono::milliseconds timeout) {
		static int resultNum{ 1 };
		Log::info(std::format("Waiting for result {}...", resultNum));
		std::unique_lock<std::mutex> lk(_mtxResult);
		if (!_cvResult.wait_for(lk, timeout, [this]() {
			return _killSwitch || !_resultQueue.empty();
			})) {
			Log::info("Waiting for result timeout");
			return maErrTimeout;
		
		};
		
		if (_killSwitch) {
			Log::info("Waiting for result stopped");
			return maErrAbort;
		}

		res = std::move(_resultQueue.front());
		_resultQueue.pop();
		if (res.first.empty()) {
			Log::info(std::format("Result {} is empty!", resultNum++));

		}
		else {
			Log::info(std::format("Result {} done!", resultNum++));
		}
		return maErrOK;
	}

	maErr Core::waitForDebug(const std::filesystem::path& path, std::chrono::milliseconds timeout) {
		if (!_debugEnabled) {
			Log::info("Saving debug data disabled");
			return maErrFunctionDeactivated;

		}

		static int debugNum{ 1 };
		Log::info(std::format("Waiting for debug data {}...", debugNum));
		std::unique_lock<std::mutex> lk(_mtxDebug);
		if (!_cvDebug.wait_for(lk, timeout, [this]() {
			return _killSwitch || !_debugQueue.empty();
			})) {
			Log::info("Waiting for debug data timeout");
			return maErrTimeout;
		}

		if (_killSwitch) {
			Log::info("Waiting for debug data stopped");
			return maErrAbort;
		}

		const auto debugData{ std::move(_debugQueue.front()) };
		_debugQueue.pop();
		// If the debug data is complex, should check if it is empty. 

		Log::info(std::format("Saving debug data {} done!", debugNum++));
		SIF::saveDebugData(path, debugData.first, debugData.second);
		return maErrOK;
	}


	maErr Core::stopWaiting() {
		_killSwitch = true;
		_cvResult.notify_all();
		_cvDebug.notify_all();
		return maErrOK;
	}
}
