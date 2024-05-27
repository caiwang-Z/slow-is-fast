#pragma once

#include "Core.h"
#include "SIFLog.h"

int main() {
	SIF::Core core;
	
	std::jthread acquisitionThread([&core](std::stop_token token) {
		core.start();
		while (true) {
			if (token.stop_requested()) {
				SIF::Logger::getInstance().info("Stop requested");
				return;
			}

			SIF::ResultSet image;
			core.waitForResult(image, std::chrono::milliseconds(500));


		}
		});

	while (true)
	{

	}

	return 0;
}