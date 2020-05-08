#include <pch.h>
#include "Application.h"

namespace rvn {
	Application::Application(std::string name)
	{
		if (_initialized) {
			LOG_ENGINE_WARN("The app \"{0}\" is already initialized", name);
		}
		Logger::init(name);
		LOG_ENGINE_TRACE("Initializing...");
		_eventHandler = createScope<EventHandler>();
		LOG_ENGINE_INFO("Initialized.");
	}
	Application::~Application()
	{
		LOG_ENGINE_TRACE("Stopping...");

		LOG_ENGINE_INFO("Stopped.");
	}
}