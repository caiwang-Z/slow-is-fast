#include <memory>
#include "FrameBuffer.h"

namespace SIF {

	using FrameBufferPtr = std::shared_ptr<FrameBuffer>;
	class ImageStream {
		friend class ImageStreamManager;
		

	public:
		virtual void start() = 0;
		virtual void stop() = 0;
	protected:
		virtual void setBuffer(const FrameBufferPtr&) = 0;
	};


}