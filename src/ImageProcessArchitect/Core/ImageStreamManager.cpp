#pragma once
#include "ImageStreamManager.h"
#include "DummyImageStream.h"
#include "SIFLog.h"

namespace SIF {
	ImageStreamManager::ImageStreamManager(const SIF::onCapturedFrame& callback) {
		_frameBuffer = std::make_unique<SIF::FrameBuffer>();
		_frameBuffer->setProcessingPipeline(callback);
		_imageStream = std::make_unique<SIF::DummyImageStream>();
		_imageStream->open();
	}

	void ImageStreamManager::start() {
		Logger::getInstance().info("Image streaming starting");
		_imageStream->setBuffer(_frameBuffer);
		_imageStream->start();
		Logger::getInstance().info("Image streaming stared.");
	}

	void ImageStreamManager::stop() {
		Logger::getInstance().info("Image streaming stopping");
		_imageStream->stop();
		_frameBuffer->stop();
		_imageStream->setBuffer(nullptr);
		Logger::getInstance().info("Image streaming stopped");
	}

	void ImageStreamManager::close() {
		Logger::getInstance().info("Image streaming resources closing");
		if (_imageStream) {
			_imageStream->close();
		}
		Logger::getInstance().info("Image streaming resources closed");
	}

}