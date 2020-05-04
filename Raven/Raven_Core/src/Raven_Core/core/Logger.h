#pragma once
#include <spdlog/spdlog.h>
#include <Raven_Core/datatypes/Memory.h>

#if defined(RELASE) || defined(DEBUG)
#define ENABLE_LOGGING
#endif

#if defined(ENABLE_LOGGING)

namespace rvn {

	class Logger {
	public:
		static void init(std::string name);
		static ref<spdlog::logger> getEngineLogger();
		static ref<spdlog::logger> getAppLogger();
	private:
		static ref<spdlog::logger> s_engineLogger;
		static ref<spdlog::logger> s_appLogger;
	};
	
}

#define LOG_ENGINE_TRACE(...)    ::rvn::Logger::getEngineLogger()->trace(__VA_ARGS__)
#define LOG_ENGINE_INFO(...)     ::rvn::Logger::getEngineLogger()->info(__VA_ARGS__)
#define LOG_ENGINE_WARN(...)     ::rvn::Logger::getEngineLogger()->warn(__VA_ARGS__)
#define LOG_ENGINE_ERROR(...)    ::rvn::Logger::getEngineLogger()->error(__VA_ARGS__)
#define LOG_ENGINE_CRITICAL(...) ::rvn::Logger::getEngineLogger()->critical(__VA_ARGS__)

#define LOG_TRACE(...)	      ::rvn::Logger::getAppLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)	      ::rvn::Logger::getAppLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)	      ::rvn::Logger::getAppLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)	      ::rvn::Logger::getAppLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)     ::rvn::Logger::getAppLogger()->critical(__VA_ARGS__)

#else

namespace rvn {
	
	class Logger {
	public:
		static void init(std::string name) {} // Does nothing if logging is disabled
	};


}

#define LOG_ENGINE_TRACE(...)
#define LOG_ENGINE_INFO(...)
#define LOG_ENGINE_WARN(...)
#define LOG_ENGINE_ERROR(...)
#define LOG_ENGINE_CRITICAL(...)

#define LOG_TRACE(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_CRITICAL(...)

#endif