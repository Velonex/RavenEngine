#pragma once
#include <Raven/application/Application.h>
#include <utility>
#include <Raven/application/MouseCodes.h>
#include <Raven/application/KeyCodes.h>

namespace rvn {
	class Input {
		friend class Application;
	public:
		static bool isKeyPressed(KeyCode key) { return _instance->_isKeyPressed(key); }
		static bool isMousePressed(MouseCode button) { return _instance->_isMousePressed(button); }
		static float getMouseX() { return _instance->_getMouseX(); }
		static float getMouseY() { return _instance->_getMouseY(); }
		static std::pair<float, float> getMousePos() { return _instance->_getMousePos(); }
	protected:
		virtual bool _isKeyPressed(KeyCode key) = 0;
		virtual bool _isMousePressed(MouseCode button) = 0;
		virtual float _getMouseX() = 0;
		virtual float _getMouseY() = 0;
		virtual std::pair<float, float> _getMousePos() = 0;
	private:
		static void setInstance(Input* instance) { _instance.reset(instance); }
		static Input* createInput(Application* appPtr);
		static scope<Input> _instance;
	};
}