#pragma once
#include <Mutex>
#include <iostream>

namespace SIF {
	enum class Level {
		Debug,
		INFO,
		WARNING,
		ERROR

	};
	class Logger {

	public:


		static Logger& getInstance() {
			static Logger instance;
			return instance;
		}
		Logger() {
			std::cout << "Logger initilized. \n";
		}
		Logger(const Logger&) = delete;
		Logger& operator= (const Logger&) = delete;
		~Logger() = default;
		void info(const std::string& message) {
			log(Level::INFO, message);
		}

		void warn(const std::string& message) {
			log(Level::WARNING, message);
		}

		void error(const std::string& message) {
			log(Level::ERROR, message);
		}

		void debug(const std::string& message) {
			log(Level::Debug, message);
		}

		void setLevel(const Level level) {
			_level = level;
		}

	private:
		Level _level = Level::INFO;
		std::mutex _mtx;
		void log(const Level level, const std::string& message);

	};

}

namespace Log {
	using Logger = SIF::Logger;

	void info(const std::string& message);
	void error(const std::string& message);
	void warn(const std::string& message);
	void debug(const std::string& message);


}