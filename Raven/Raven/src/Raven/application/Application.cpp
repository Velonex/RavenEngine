#include <pch.h>
#include "Application.h"
#include <Raven/application/Input.h>

namespace rvn {
	Application::Application(std::string name)
	{
		if (_initialized) {
			LOG_ENGINE_WARN("The app \"{0}\" is already initialized", name);
		}
		Logger::init(name);
		LOG_ENGINE_TRACE("Initializing...");
		_eventHandler = createScope<EventHandler>();
		_eventHandler->subscribe(this, EventType::ALL);
		WindowProps props(1280, 720, name);
		_window.reset(Window::createWindow(props, this));
		Input::setInstance(Input::createInput(this));
		LOG_ENGINE_INFO("Initialized.");
		_initialized = true;
	}
	void Application::run()
	{
		if (!_initialized) {
			LOG_ENGINE_ERROR("The constructor has to be called first");
			return;
		}
		while (_running) {
			_window->onUpdate();
		}
	}
	Application::~Application()
	{
		LOG_ENGINE_TRACE("Stopping...");
		LOG_ENGINE_INFO("Stopped.");
	}
	void Application::onEvent(Event* e)
	{
		switch (e->getType()) {
			case EventType::EVENT_WINDOW_CLOSE: {
				_running = false;
				break;
			}
		}
	}
}