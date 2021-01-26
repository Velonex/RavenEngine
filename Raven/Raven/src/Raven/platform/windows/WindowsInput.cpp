#include <pch.h>
#include "Raven/application/Input.h"
#if defined(PLATFORM_WINDOWS)
#include <GLFW/glfw3.h>

namespace rvn {

	bool Input::isKeyPressed(KeyCode key) {
		return glfwGetKey(static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow()), (int)key) == GLFW_PRESS;
	}
	bool Input::isMousePressed(MouseCode button) {
		return glfwGetMouseButton(static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow()), (int)button) == GLFW_PRESS;
	}
	float Input::getMouseX() {
		double x, y;
		glfwGetCursorPos(static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow()), &x, &y);
		return (float)x;
	}
	float Input::getMouseY() {
		double x, y;
		glfwGetCursorPos(static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow()), &x, &y);
		return (float)y;
	}
	glm::vec2 Input::getMousePos() {
		double x, y;
		glfwGetCursorPos(static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow()), &x, &y);
		return { (float)x, (float)y };
	}
}
#endif