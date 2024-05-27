#pragma once
#include "DummyImageStream.h"
#include "FrameBuffer.h"

namespace SIF {
	class ImageStreamManager {
	public:
		ImageStreamManager(const SIF::onCapturedFrame& callback);
		void start();
		void stop();
		void close();
	private:
		FrameBufferPtr _frameBuffer;
		std::unique_ptr<DummyImageStream> _imageStream;




	};

}