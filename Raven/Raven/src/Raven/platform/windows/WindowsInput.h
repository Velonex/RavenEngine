#pragma once
#include <Raven_Core/core/Platform.h>

#if defined(PLATFORM_WINDOWS)
#include <Raven/application/Window.h>
#include <Raven/application/Input.h>
#include <Raven_Core/datatypes/Memory.h>
#include <GLFW/glfw3.h>

namespace rvn {

	class WindowsInput : public Input
	{
	public:
		WindowsInput(Application* appPtr);
		~WindowsInput();
	protected:
		virtual bool _isKeyPressed(KeyCode keyCode) override;
		virtual bool _isMousePressed(MouseCode button) override;
		virtual float _getMouseX() override;
		virtual float _getMouseY() override;
		virtual std::pair<float, float> _getMousePos() override;
	private:
		GLFWwindow* _window;
	private:
		static WindowsInput* _instance;
	};

}

#endif