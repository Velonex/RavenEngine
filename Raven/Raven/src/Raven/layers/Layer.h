#pragma once
#include <Raven_Core/event/EventSystem.h>

namespace rvn {

	class Layer {
	public:
		Layer(std::string name = "Layer") : _name(name) {}
		~Layer() {}
		virtual void onAttach() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onEvent(Event* e) {}
		virtual void onDetach() {}
		virtual void onImGuiRender() {}
		const std::string& getName() const { return _name; }
	private:
		std::string _name;
	};

}