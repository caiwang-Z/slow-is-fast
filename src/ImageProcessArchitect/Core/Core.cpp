#include "Core.h"
#include "SIFLog.h"


namespace SIF {

	Core::Core() {
		_imageProcessingPipeline = std::make_unique<SIF::ImageProcessingPipeline>();
		_imageProcessingPipeline->setOnProcessFinished({ [this](SIF::ResultSet&& featureImage) {
			std::lock_guard<std::mutex> lk(_mtxResult);
			if (_resultQueue.size() >= g_maxQueueSize) {
				Log::info("Result image queue is full!!!");
			}
			else {
				_resultQueue.push(featureImage);
			}
			_cvResult.notify_all();
			} });
		_imageStreamManager = std::make_unique<ImageStreamManager>(std::bind_front(&ImageProcessingPipeline::process, &(*_imageProcessingPipeline)));

	}

	void Core::start() {
		_imageStreamManager->start();
		Log::info("Core started");
	}

	void Core::stop() {
		Log::info("Core stopping...");
		_imageStreamManager->stop();
		Log::info("Core stopped");
	
	}

	void Core::waitForResult(ResultSet& res, std::chrono::milliseconds timeout) {
		static int resultNum{ 1 };
		Log::info(std::format("Waiting for result {}...", resultNum));
		std::unique_lock<std::mutex> lk(_mtxResult);
		if (!_cvResult.wait_for(lk, timeout, [this]() {
			return _killSwitch || !_resultQueue.empty();
			})) {
			Log::info("Waiting for result timeout");
			return;
		
		};
		
		if (_killSwitch) {
			Log::info("Waiting for result is stopped");
			return;
		}

		res = std::move(_resultQueue.front());
		_resultQueue.pop();
		if (res.first.empty()) {
			Log::info(std::format("Result {} is empty!", resultNum++));

		}
		else {
			Log::info(std::format("Result {} done!", resultNum++));
		}



	}

	void Core::stopWaiting() {
		_killSwitch = true;
		_cvResult.notify_all();
	
	}
}
