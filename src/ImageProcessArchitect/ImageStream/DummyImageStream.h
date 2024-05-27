#pragma once 
#include "IImageStream.h"

namespace SIF {
	constexpr auto timeSleepInMilliseconds{ 500 };
	inline std::string getCurrentDateTime();
	class DummyImageStream : public ImageStream {
		friend class ImageStreamManager;

	public:
		DummyImageStream() = default;
		void open() override;
		void start() override;
		void stop() override;
		bool endOfStream() override;
		void close() override;
	protected:
		void setBuffer(const FrameBufferPtr&) override;

	private:
		FrameBufferPtr _frameBuffer;
		std::jthread _imageStreamer;
		std::atomic_bool _endOfStream = false;

	};

}