#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Raven/rendering/GraphicsContext.h>

namespace rvn {

	class OpenGLGraphicsContext : public GraphicsContext {
	public:
		OpenGLGraphicsContext(GLFWwindow* window);
		virtual void init() override;
		virtual void swapBuffers() override;
	private:
		GLFWwindow* _window;
	};

}
