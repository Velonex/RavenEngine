#include <pch.h>
#include "Application.h"
#include <Raven/application/Input.h>
#include <chrono>
#include "Timestep.h"

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
		_layerStack = createScope<LayerStack>();
		LOG_ENGINE_INFO("Initialized.");
		_initialized = true;
	}
	void Application::run()
	{
		if (!_initialized) {
			LOG_ENGINE_ERROR("The constructor has to be called first");
			return;
		}
		Timestep timestep;
		std::chrono::time_point<std::chrono::steady_clock> lastFrame = std::chrono::time_point<std::chrono::steady_clock>();
		std::chrono::time_point<std::chrono::steady_clock> now;
		while (_running) {
			// Calculate time passed
			now = std::chrono::steady_clock::now();
			timestep = ((float)(now - lastFrame).count()) / 1000000000;
			lastFrame = now;
			// Update all layers
			for (auto it = _layerStack->rbegin(); it < _layerStack->rend(); it++) {
				(*it)->onUpdate(timestep);
			}
			// Update window
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
		for (auto it = _layerStack->rbegin(); it < _layerStack->rend(); it++) {
			(*it)->onEvent(e);
			if (e->handled) break;

		}
		switch (e->getType()) {
			case EventType::EVENT_WINDOW_CLOSE: {
				_running = false;
				break;
			}
		}
	}
}