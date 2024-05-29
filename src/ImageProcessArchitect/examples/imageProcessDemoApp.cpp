#pragma once

#include "Core.h"
#include "SIFLog.h"

constexpr auto timeoutValue{ 100000 };

std::mutex mtxCommand;
std::condition_variable cv;
std::atomic_bool paused{false};

void acquisition(std::stop_token token) {
	SIF::Core core;
	core.start();

	while (true) {
		if (token.stop_requested()) {
			Log::info("Stop requested");
			return;
		}

		std::unique_lock<std::mutex> lk(mtxCommand);
		cv.wait(lk, []() {return !paused.load(); });

		SIF::ResultSet image;
		core.waitForResult(image, std::chrono::milliseconds(timeoutValue));


	}

}

void commandListenerThread(std::jthread& t) {
	while (true) {
		char command;
		std::cin >> command;
		if (command == 'q') {
			t.request_stop();
			cv.notify_all();
			Log::info("Exiting program");
			break;
		}
		else if (command == 'p') {
			paused = true;
			Log::info("Program paused");
			cv.notify_all();
		}
		else if (command == 'c') {
			paused = false;
			cv.notify_all();
			Log::info("Program continued");
		}
		else {
			Log::warn("Invalid command. 'q' to quit, 'p' to pause, 'c' to continue");
		
		}
	
	}

}

int main() {
	
	std::jthread acquisitionTask(acquisition);
	std::jthread commandThread(commandListenerThread, std::ref(acquisitionTask));

	if (acquisitionTask.joinable()) {
		acquisitionTask.join();
	}

	if (acquisitionTask.joinable()) {
		acquisitionTask.join();
	}

	return 0;
}