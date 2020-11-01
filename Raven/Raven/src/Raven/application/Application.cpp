#include <pch.h>
#include "Application.h"
#include <Raven/application/Input.h>
#include <Raven_Core/utility/Timer.h>
#include "Timestep.h"
#include <imgui.h>
#include <Raven/rendering/RenderCommand.h>
#include <Raven/rendering/Renderer.h>
#include <Raven/rendering/Renderer2D.h>

namespace rvn {
	
	Application* Application::_instance = nullptr;

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
		Renderer::init();
		Renderer2D::init();
		_layerStack = createScope<LayerStack>();
		LOG_ENGINE_INFO("Initialized.");
		_initialized = true;
		_instance = this;
	}
	void Application::run()
	{
		if (!_initialized) {
			LOG_ENGINE_ERROR("The constructor has to be called first");
			return;
		}
		_layerStack->pushOverlay(_imGuiLayer = new ImGuiLayer(&(this->getWindow())));
		Timestep timestep;
		using clock = std::chrono::steady_clock;
		std::chrono::time_point<clock> lastFrame = std::chrono::time_point<clock>();
		std::chrono::time_point<clock> now;

		Timer timer;
		//int lastFPS = 0;

		while (_running) {
			// Calculate time passed
			now = std::chrono::steady_clock::now();
			std::chrono::duration<float, std::micro> duration(now - lastFrame);
			timestep = duration.count() / 1000000;
			lastFrame = now;
			// Update all layers
			for (auto it = _layerStack->rbegin(); it < _layerStack->rend(); it++) {
				(*it)->onUpdate(timestep);
			}
			_imGuiLayer->beginFrame();
			{
				//ImGui::Begin("Renderer");
				//if (timer.hasReached(100)) {
				//	lastFPS = (int)(1 / timestep.getSeconds());
				//}
				//ImGui::Text("FPS: %d", lastFPS);
				//ImGui::End();
			}
			{
				for (auto it = _layerStack->rbegin(); it < _layerStack->rend(); it++) {
					(*it)->onImGuiRender();
				}
			}
			_imGuiLayer->endFrame();
			// Update window
			_window->onUpdate();
		}
	}
	Application::~Application()
	{
		LOG_ENGINE_TRACE("Stopping...");
		if (_instance == this) _instance = nullptr;
		for (auto it = _layerStack->rbegin(); it < _layerStack->rend(); it++) {
			(*it)->onDetach();
			delete (*it);
		}
		Renderer::shutdown();
		Renderer2D::shutdown();
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
			case EventType::EVENT_WINDOW_RESIZE: {
				onWindowResize((WindowResizeEvent*)e);
			}

		}
	}
	void Application::close()
	{
		_running = false;
	}
	void Application::onWindowResize(WindowResizeEvent* e)
	{
		Renderer::onWindowResize(e->getWidth(), e->getHeight());
	}
}