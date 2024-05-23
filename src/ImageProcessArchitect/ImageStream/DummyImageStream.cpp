#include "DummyImageStream.h"
#include "SIFLog.h"
#include <thread>

void SIF::DummyImageStream::setBuffer(const FrameBufferPtr& buffer) {
	_frameBuffer = buffer;
}

void SIF::DummyImageStream::start() {
	_imageStreamer = std::jthread([this](std::stop_token token) {
		SIF::Logger::getInstance().info("Start image streaming / generating miner data\n");
		int i = 1;
		while (!token.stop_requested()) {
			MinerData* data = new MinerData();
			data->id = i;
			data->diamondAmount = i;
			data->goldAmount = i * 10;

			if (_frameBuffer) {
				while (!_frameBuffer->writeBuffer(*data)) {
					SIF::Logger::getInstance().debug("No buffer free.");
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				SIF::Logger::getInstance().info(std::format("Generating miner data finished. ID: {}, \
					Diamond amount: {}, gold amount: {}", data->id, data->diamondAmount, data->goldAmount));
			}

			++i;
			std::this_thread::sleep_for(std::chrono::milliseconds(timeSleepInMilliseconds));
		}
		
		});


}

void SIF::DummyImageStream::stop() {
	_imageStreamer.request_stop();
	_imageStreamer.join();
}