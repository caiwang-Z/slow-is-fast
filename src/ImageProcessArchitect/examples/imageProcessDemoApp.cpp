#pragma once

#include "Core.h"
#include "SIFLog.h"
#include <filesystem>

constexpr auto timeoutValue{ 100000 };

std::mutex mtxCommand;
std::condition_variable cv;
std::atomic_bool paused{false};
std::atomic_bool killSwich{ false };
std::filesystem::path debugFolder{ "debugData" };

void acquisition(SIF::Core& core, std::stop_token token) {

	while (true) {
		if (token.stop_requested()) {
			Log::info("Acquisition Stop requested");
			return;
		}

		std::unique_lock<std::mutex> lk(mtxCommand);
		cv.wait(lk, []() {return !paused.load() || killSwich.load(); });

		if (killSwich.load()) {
			Log::info("Program (acquisition) exit requested");
			return;
		}

		SIF::ResultSet image;
		core.waitForResult(image, std::chrono::milliseconds(timeoutValue));


	}

}

void debugTask(SIF::Core& core, std::stop_token token) {
	core.enableDebug();
	while (true) {
		if (token.stop_requested()) {
			Log::info("Debug Stop requested");
			return;
		}

		std::unique_lock<std::mutex> lk(mtxCommand);
		cv.wait(lk, []() {return !paused.load() || killSwich.load(); });

		if (killSwich.load()) {
			Log::info("Program (debug) exit requested");
			return;
		}

		core.waitForDebug(debugFolder, std::chrono::milliseconds(timeoutValue));


	}

}

void commandListenerThread() {
	while (true) {
		char command;
		std::cin >> command;
		if (command == 'q') {
			killSwich = true;
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
	SIF::Core core;
	core.start();
	std::jthread acquisitionTask(acquisition, std::ref(core), std::stop_token{});
	std::jthread debugThread(debugTask, std::ref(core), std::stop_token{});
	// By default, arguments passed to threaded functions are passed by value, which means that a copy of the argument is made. However, in some cases, we want to pass arguments by reference to avoid unnecessary copying, 
	// or because we need to share and modify the same object in a thread
	std::jthread commandThread(commandListenerThread);

	if (commandThread.joinable()) {
		commandThread.join();
	}

	if (acquisitionTask.joinable()) {
		acquisitionTask.join();
	}

	if (debugThread.joinable()) {
		debugThread.join();
	}

	return 0;
}