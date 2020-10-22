#include <pch.h>
#include "WindowsInput.h"

#if defined(PLATFORM_WINDOWS)
namespace rvn {
	scope<Input> Input::_instance;
	WindowsInput::WindowsInput(Application* appPtr)
	{
		_window = (GLFWwindow*)(appPtr->getWindow().getNativeWindow());
	}

	WindowsInput::~WindowsInput()
	{
	}

	bool WindowsInput::_isKeyPressed(KeyCode keyCode)
	{
		return glfwGetKey(_window, (int)keyCode) == GLFW_PRESS;
	}

	bool WindowsInput::_isMousePressed(MouseCode mouseCode)
	{
		return glfwGetMouseButton(_window, (int)mouseCode) == GLFW_PRESS;
	}

	float WindowsInput::_getMouseX()
	{
		double x, y;
		glfwGetCursorPos(_window, &x, &y);
		return (float)x;
	}

	float WindowsInput::_getMouseY()
	{
		double x, y;
		glfwGetCursorPos(_window, &x, &y);
		return (float)y;
	}

	std::pair<float, float> WindowsInput::_getMousePos()
	{
		double x, y;
		glfwGetCursorPos(_window, &x, &y);
		return { (float)x, (float)y };
	}

}
#endif