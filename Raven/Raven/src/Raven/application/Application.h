#pragma once

#include <string>
#include <Raven_Core/core/Logger.h>
#include <Raven_Core/event/EventSystem.h>
#include <Raven_Core/datatypes/Memory.h>

namespace rvn {

	class Application {
	public:
		Application(std::string name);
		~Application();
	private:
		bool _initialized = false;
		scope<EventHandler> _eventHandler;
	};

}
