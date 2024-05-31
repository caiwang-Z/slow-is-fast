#include "DummyImageStream.h"
#include "SIFLog.h"
#include <thread>
#include <format>
#include <chrono>
#include <iomanip>
#include <sstream>

void SIF::DummyImageStream::setBuffer(const FrameBufferPtr& buffer) {
	_frameBuffer = buffer;
}

void SIF::DummyImageStream::open() {
	Log::info("Camera opening...");
	Log::info("Camera opend.");
}

inline std::string SIF::getCurrentDateTime() {
	auto now = std::chrono::system_clock::now();

	std::time_t now_tt = std::chrono::system_clock::to_time_t(now);
	std::tm now_tm = *std::localtime(&now_tt);

	std::ostringstream oss;
	oss << std::put_time(&now_tm, "%Y-%m-%d_%H-%M-%S");

	return oss.str();

}


void SIF::DummyImageStream::start() {
	// prevent multiple calling
	const auto isRunning{ _imageStreamer.joinable() };
	if (isRunning) {
		return;
	}

	_imageStreamer = std::jthread([this](std::stop_token token) {
		Log::info("Start image streaming / generating miner data\n");
		int i = 1;
		while (!token.stop_requested()) {
			MinerData* data = new MinerData();
			data->id = i;
			data->diamondAmount = i;
			data->goldAmount = i * 10;
			data->timestamp = getCurrentDateTime();
			if (_frameBuffer) {
				while (!_frameBuffer->writeBuffer(*data)) {
					Log::debug("No buffer free.");
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				Log::info(std::format("Generating miner data finished. ID: {}, Diamond amount: {}, gold amount: {}, timestamp: {}", data->id, data->diamondAmount, data->goldAmount, data->timestamp));
			}

			++i;
			std::this_thread::sleep_for(std::chrono::milliseconds(timeSleepInMilliseconds));
			_endOfStream = true;
		}
		
		});


}

void SIF::DummyImageStream::stop() {
	_imageStreamer.request_stop();
	_imageStreamer.join();
	_endOfStream = false;
}

bool SIF::DummyImageStream::endOfStream() {
	return _endOfStream;
}

void SIF::DummyImageStream::close() {
	Log::info("Camera resources closing...");
	Log::info("Camera resources closing");

}