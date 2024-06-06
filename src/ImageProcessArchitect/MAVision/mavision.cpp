#include "SIFLog.h"
#include "mavision.h"
#include <memory>
// This project could no be built. Error: Cannot include file: DummyImageOperation.h: No such file or directory.
inline maErr catchUnknown(const std::function<maErr(void)>& f) {
	try {
		f();
	}
	catch (const std::exception& e) {
		const std::string errorMessage = e.what();
		Log::error(std::format("Caught exception: {}", errorMessage));
		return maErrUnknown;
	}
	catch (...) {
		Log::error("Caught unknown exception");
		return maErrUnknown;
	}

}

std::unique_ptr<SIF::Core> core;

MAVISION_DLL_EXPORT maErr maStart() {
	return catchUnknown([]() {
		if (core) {
			core.reset();
		}
		core = std::make_unique<SIF::Core>();
		return core->start();
		});
}

MAVISION_DLL_EXPORT maErr maSaveDebugData(const char* debugDataFolder, int timeoutMS) {
	return catchUnknown([debugDataFolder, timeoutMS]() {
		return core->waitForDebug(debugDataFolder, std::chrono::milliseconds(timeoutMS));

		});
}

MAVISION_DLL_EXPORT maErr maStop() {
	return catchUnknown([]() {
		return core->stop();
		});
}

MAVISION_DLL_EXPORT maErr maEnableDebug() {
	return catchUnknown([]() {
		return core->enableDebug();
		});
}

MAVISION_DLL_EXPORT maErr maDisableDebug() {
	return catchUnknown([]() {
		return core->disableDebug();
		});
}

MAVISION_DLL_EXPORT maErr maStopWaiting() {
	return catchUnknown([]() {
		return core->stopWaiting();

		});
}