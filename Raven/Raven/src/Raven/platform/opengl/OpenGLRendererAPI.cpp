#include <pch.h>
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>
#include <Raven_Core/core/Logger.h>

namespace rvn {



	void OpenGLRendererAPI::init()
	{
		#if defined (DEBUG)
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		#endif
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::setClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRendererAPI::draw(const ref<VertexArray> vertexArray, std::uint32_t indexCount)
	{
		std::uint32_t count = indexCount ? indexCount : vertexArray->getIndexBuffer()->getCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLRendererAPI::setViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height)
	{
		glViewport(x, y, width, height);
	}
	void OpenGLRendererAPI::OpenGLMessageCallback(unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:			LOG_ENGINE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:			LOG_ENGINE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:				LOG_ENGINE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	LOG_ENGINE_TRACE(message); return;
		}
	}
}