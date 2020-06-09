#include <pch.h>
#include "Shader.h"
#include <Raven/platform/opengl/OpenGLShader.h>

namespace rvn {
	ref<Shader> Shader::createShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource, const std::string& name)
	{
		switch (RendererAPI::getRendererApi()) {
		case RendererAPI::API::OpenGL:
			return createRef<OpenGLShader>(vertexShaderSource, fragmentShaderSource, name);
		}
		ASSERT(false, "Unsupported RendererAPI");
		return nullptr;
	}
}