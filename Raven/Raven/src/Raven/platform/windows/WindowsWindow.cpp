#include <pch.h>
#include "WindowsWindow.h"

#if defined(PLATFORM_WINDOWS)
#include <Raven_Core/core/Assert.h>
#include <Raven/events/KeyboardEvents.h>
#include <Raven/events/MouseEvents.h>
#include <Raven/events/WindowEvents.h>
#include <Raven/application/Application.h>

namespace rvn {

	static uint8_t s_glfwWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		LOG_ENGINE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props, Application* appPtr)
	{
		_init(props, appPtr);
	}
	WindowsWindow::~WindowsWindow()
	{
		_shutdown();
	}


	void WindowsWindow::_init(const WindowProps& props, Application* appPtr)
	{
		_windowData.handler = &(appPtr->getEventHandler());
		_windowData.title = props.title;
		_windowData.width = props.width;
		_windowData.height = props.height;

		LOG_ENGINE_INFO("Creating Window: \"{0}\" ({1}, {2})", props.title, props.width, props.height);

		if (s_glfwWindowCount == 0) {
			LOG_ENGINE_TRACE("Initializing GLFW...");
			int success = glfwInit();
			ASSERT(success, "Couldn't initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		//#if defined(DEBUG)
		//	if (Renderer::getApi() == RendererAPI::API::OpenGL)
		//		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		//#endif
		_window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);
		s_glfwWindowCount++;

		_context.reset(GraphicsContext::createGraphicsContext(_window));
		_context->init();
		glfwSetWindowUserPointer(_window, &_windowData);
		setVSync(true);

		glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
			_WindowData& data = *(_WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent* event = new WindowCloseEvent();
			data.handler->dispatchEvent(event);
			});
		glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
			_WindowData& data = *(_WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;
			WindowResizeEvent* event = new WindowResizeEvent(width, height);
			data.handler->dispatchEvent(event);
			});
		glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int keycode) {
			_WindowData& data = *(_WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent* event = new KeyTypedEvent(static_cast<KeyCode>(keycode));
			data.handler->dispatchEvent(event);
			});
		glfwSetKeyCallback(_window, [](GLFWwindow* window, int keycode, int scancode, int action, int mods) {
			_WindowData& data = *(_WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
			case GLFW_PRESS: {
				KeyPressedEvent* ev = new KeyPressedEvent(static_cast<KeyCode>(keycode), 0);
				data.handler->dispatchEvent(ev);
				break;
			}
			case GLFW_REPEAT: {
				KeyPressedEvent* ev = new KeyPressedEvent(static_cast<KeyCode>(keycode), 1);
				data.handler->dispatchEvent(ev);
				break;
			}
			case GLFW_RELEASE: {
				KeyReleasedEvent* ev = new KeyReleasedEvent(static_cast<KeyCode>(keycode));
				data.handler->dispatchEvent(ev);
				break;
			}
			}
			});
		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
			_WindowData& data = *(_WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
			case GLFW_PRESS: {
				MouseButtonPressedEvent* event = new MouseButtonPressedEvent(static_cast<MouseCode>(button));
				data.handler->dispatchEvent(event);
				break;
			}
			case GLFW_RELEASE: {
				MouseButtonReleasedEvent* event = new MouseButtonReleasedEvent(static_cast<MouseCode>(button));
				data.handler->dispatchEvent(event);
				break;
			}
			}
			});
		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double x, double y) {
			_WindowData& data = *(_WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent* event = new MouseMovedEvent((float)x, (float)y);
			data.handler->dispatchEvent(event);
			});
		glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOff, double yOff) {
			_WindowData& data = *(_WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent* event = new MouseScrolledEvent((float)xOff, (float)yOff);
			data.handler->dispatchEvent(event);
			});
	}

	void WindowsWindow::onUpdate()
	{
		glfwPollEvents();
		_context->swapBuffers();
	}

	void WindowsWindow::_shutdown()
	{
		glfwDestroyWindow(_window);
		--s_glfwWindowCount;
		if (s_glfwWindowCount == 0)
			glfwTerminate();
	}
	unsigned int WindowsWindow::getWidth() const
	{
		return _windowData.width;
	}

	unsigned int WindowsWindow::getHeight() const
	{
		return _windowData.height;
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		_windowData.vsync = enabled;
	}

	bool WindowsWindow::getVSync() const
	{
		return _windowData.vsync;
	}

	inline void* WindowsWindow::getNativeWindow() const
	{
		return (void*)_window;
	}

	Window* Window::createWindow(const WindowProps& props, Application* appPtr) {
		return new WindowsWindow(props, appPtr);
	}
}

#endif