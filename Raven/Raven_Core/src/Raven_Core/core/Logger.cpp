#include "Logger.h"
#if defined(ENABLE_LOGGING)
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <ctime>
#include <chrono>
#include <string>


namespace rvn {


	ref<spdlog::logger> Logger::s_engineLogger = nullptr;
	ref<spdlog::logger> Logger::s_appLogger = nullptr;

	void Logger::init(std::string name)
	{
		if (s_engineLogger != nullptr) {
			LOG_ENGINE_WARN("Logger already initialized");
			return;
		}
		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		// Generating name for logfile
		std::time_t t = std::time(0);
		std::tm now = *std::localtime(&t);
		std::string filename = "Raven-" + std::to_string(now.tm_mon + 1) + std::to_string(now.tm_mday) + std::to_string(now.tm_year + 1900) + "-" +
			std::to_string(now.tm_hour) + "-" + std::to_string(now.tm_min) + ".log";
		sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(std::string(filename), false));

		// Pattern for console output
		sinks[0]->set_pattern("[%T][%^%l%$] %n: %v");
		// Pattern for logfile output (has no color codes)
		sinks[1]->set_pattern("[%T][%l] %n: %v");

		s_engineLogger = std::make_shared<spdlog::logger>("Raven", sinks.begin(), sinks.end());
		spdlog::register_logger(s_engineLogger);
		s_engineLogger->set_level(spdlog::level::trace);
		s_engineLogger->flush_on(spdlog::level::trace);

		s_appLogger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
		spdlog::register_logger(s_appLogger);
		s_appLogger->set_level(spdlog::level::trace);
		s_appLogger->flush_on(spdlog::level::trace);
	}

	ref<spdlog::logger> Logger::getEngineLogger()
	{
		return s_engineLogger;
	}

	ref<spdlog::logger> Logger::getAppLogger()
	{
		return s_appLogger;
	}

}
#endif