#include "SIFLog.h"

void SIF::Logger::log(const Level level, const std::string& message) {
	if (level >= _level) {
		std::string prefix;
		switch (level) {
		case Level::INFO:
			prefix = "[info] - ";
			break;
		case Level::WARNING:
			prefix = "[warn] - ";
			break;
		case Level::ERROR:
			prefix = "[error] - ";
			break;
		case Level::Debug:
			prefix = "[debug] - ";
			break;
		default:
			prefix = "[unknown] - ";
		}

		std::lock_guard<std::mutex> lk(_mtx);
		std::cout << prefix << message << "\n";

	}
}

//static SIF::Logger& logger = SIF::Logger::getInstance();

namespace Log {
	void info(const std::string& message) {
		Logger::getInstance().info(message);

	}

	void warn(const std::string& message) {
		Logger::getInstance().warn(message);

	}

	void error(const std::string& message) {
		Logger::getInstance().error(message);
	}

	void debug(const std::string& message) {
		Logger::getInstance().debug(message);
	}


}