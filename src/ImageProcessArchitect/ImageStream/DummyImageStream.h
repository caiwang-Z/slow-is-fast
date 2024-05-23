#include "IImageStream.h"

namespace SIF {
	constexpr auto timeSleepInMilliseconds{ 500 };

	class DummyImageStream : public ImageStream {
	public:
		DummyImageStream() = default;
		void start() override;
		void stop() override;
	protected:
		void setBuffer(const FrameBufferPtr&) override;

	private:
		FrameBufferPtr _frameBuffer;
		std::jthread _imageStreamer;
	};


}