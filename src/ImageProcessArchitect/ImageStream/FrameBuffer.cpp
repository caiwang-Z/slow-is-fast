#include "FrameBuffer.h"
#include <thread>
#include "SIFLog.h"
#include <string>
#include <format>

bool SIF::FrameBuffer::writeBuffer(MinerData& data) {
	std::lock_guard<std::mutex> lk(_mtxFrameBuffer);
	// const iterator: cannot modify itself, but can modify the contents of the object it points to
	const auto nextFreeIt{ std::find_if(_frameBuffer.begin(), _frameBuffer.end(), [](auto& frame) {return frame.isFree(); }) };
	if (nextFreeIt != _frameBuffer.end()) {
		nextFreeIt->data = data;
		nextFreeIt->future = std::async(std::launch::async, [this, &data]() {
			_onCapturedFrame(data);
			// TODO complex data should be free after processing
			// TODO free data
		});
		const auto index{ static_cast<int>(std::distance(_frameBuffer.begin(), nextFreeIt)) };
		
		Log::debug(std::string("Wrote to frame buffer ") + std::to_string(index));
		return true;
		

	}
	else {
		Log::debug("All frame buffer are in use.");
		return false;
	}

}

void SIF::FrameBuffer::setProcessingPipeline(const onCapturedFrame& ocf) {
	_onCapturedFrame = ocf;

}

void SIF::FrameBuffer::printBufferStatus() {
	for (int i = 0; i < _frameBuffer.size(); ++i) {
		if (_frameBuffer[i].isFree()) {
			Log::info(std::format("Buffer {} free", std::to_string(i)));
		}
		else {
			Log::info(std::format("Buffer {} not free", std::to_string(i)));

		}
	
	}

}

void SIF::FrameBuffer::stop() {
	for (auto& buffer : _frameBuffer) {
		if (buffer.future.valid()) {
			buffer.future.wait();
		}
	}

}