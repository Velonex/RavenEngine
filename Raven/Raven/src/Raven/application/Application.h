#pragma once

#include <string>
#include <Raven_Core/core/Logger.h>
#include <Raven_Core/event/EventSystem.h>
#include <Raven_Core/datatypes/Memory.h>
#include <Raven/application/Window.h>
#include <Raven/layers/LayerStack.h>

namespace rvn {

	class Application : public EventListener {
	public:
		Application(std::string name);
		void run();
		~Application();
		EventHandler& getEventHandler() const { return *_eventHandler; }
		Window& getWindow() const { return *_window; }
		LayerStack& getLayerStack() const { return *_layerStack; }
		virtual void onEvent(Event* e) override;
	private:
		bool _initialized = false;
		bool _running = true;
		scope<EventHandler> _eventHandler;
		scope<Window> _window;
		scope<LayerStack> _layerStack;
	};

}
