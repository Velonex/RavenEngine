#include <pch.h>
#include "OpenGLGraphicsContext.h"
#include <Raven_Core/core/Assert.h>

namespace rvn {

	OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* window)
	{
		_window = window;
	}

	void OpenGLGraphicsContext::init()
	{
		glfwMakeContextCurrent(_window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ASSERT(status, "Couldn't initialize Glad");
		LOG_ENGINE_INFO("Renderer: OpenGL {0}", glGetString(GL_VERSION));
		LOG_ENGINE_INFO("GPU: {0}, {1}", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
	}

	void OpenGLGraphicsContext::swapBuffers()
	{
		glfwSwapBuffers(_window);
	}

	GraphicsContext* GraphicsContext::createGraphicsContext(void* window) {
		return new OpenGLGraphicsContext((GLFWwindow*)window);
	}
}