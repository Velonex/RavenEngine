#pragma once

#include <string>
#include <Raven_Core/core/Logger.h>
#include <Raven_Core/event/EventSystem.h>
#include <Raven_Core/datatypes/Memory.h>
#include <Raven/application/Window.h>
#include <Raven/layers/LayerStack.h>
#include <Raven/imgui/ImGuiLayer.h>
#include <Raven/events/WindowEvents.h>

namespace rvn {

	class Application : public EventListener {
	public:
		Application(std::string name);
		void run();
		~Application();
		EventHandler& getEventHandler() const { return *_eventHandler; }
		Window& getWindow() const { return *_window; }
		LayerStack& getLayerStack() const { return *_layerStack; }
		ImGuiLayer* getImGuiLayer() { return _imGuiLayer; }
		virtual void onEvent(Event* e) override;
		void close();
		static Application& get() { return *_instance; }
	private:
		void onWindowResize(WindowResizeEvent* e);
	private:
		bool _initialized = false;
		bool _running = true;
		scope<EventHandler> _eventHandler;
		scope<Window> _window;
		scope<LayerStack> _layerStack;
		ImGuiLayer* _imGuiLayer;
		static Application* _instance;
	};

}
