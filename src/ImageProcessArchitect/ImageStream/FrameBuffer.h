#pragma once
#include <chrono>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <array>
#include <string>

namespace SIF {

	struct MinerData {
		//std::string timestamp;
		//std::string minerName;
		size_t id;
		size_t goldAmount;
		size_t diamondAmount;
		//std::string location;


	};

	struct Frame {
		MinerData data;
		std::future<void> future;
		bool isFree() {
			// future.valid: There are no asynchronous tasks associated with it or the task has been completed and the data is available
			// future.wait_for(std::chrono::seconds(0)) == std::future_status::ready: to check if the future is complete.
			// This method is suitable for scenarios where you need to check the status of an asynchronous task frequently, 
			// but don't want to block the current thread
			return !future.valid() || future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
		}

	};

	using onCapturedFrame = std::function<void(MinerData&)>;
	static constexpr int g_bufferCount{ 10 };
	class FrameBuffer {
		friend class ImageStreamManager;
		friend class DummyImageStream;
	protected:
		bool writeBuffer(MinerData& data);
		void setProcessingPipeline(const onCapturedFrame& ocf);
		void printBufferStatus();
		void stop();

	private:
		std::mutex _mtxFrameBuffer;
		std::array<Frame, g_bufferCount> _frameBuffer;
		onCapturedFrame _onCapturedFrame;



	};

}