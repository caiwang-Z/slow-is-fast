#pragma once
#include <memory>
#include "FrameBuffer.h"

namespace SIF {

	using FrameBufferPtr = std::shared_ptr<FrameBuffer>;
	class ImageStream {
	public:
		virtual void open() = 0;
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual bool endOfStream()= 0;
		virtual void close() = 0;
	protected:
		virtual void setBuffer(const FrameBufferPtr&) = 0;
	};
}