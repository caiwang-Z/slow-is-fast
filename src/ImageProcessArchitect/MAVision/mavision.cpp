#include "Core.h"
#include "SIFLog.h"
#include "mavision.h"
#include <memory>

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