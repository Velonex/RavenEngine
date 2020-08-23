#include <pch.h>
#include "Shader.h"
#include <Raven/platform/opengl/OpenGLShader.h>

namespace rvn {
	ref<Shader> Shader::create(const std::string& vertexShaderSource, const std::string& fragmentShaderSource, const std::string& name)
	{
		switch (RendererAPI::getRendererApi()) {
		case RendererAPI::API::OpenGL:
			return createRef<OpenGLShader>(vertexShaderSource, fragmentShaderSource, name);
		}
		ASSERT(false, "Unsupported RendererAPI");
		return nullptr;
	}
	void ShaderLib::add(ref<Shader> shader)
	{
		add(shader->getName(), shader);
	}
	void ShaderLib::add(const std::string& name, ref<Shader> shader)
	{
		ASSERT(!exists(name), "Shader with name '{0}' already exists", name);
		_shaders[name] = shader;
	}
	bool ShaderLib::exists(const std::string& name) const
	{
		return _shaders.find(name) != _shaders.end;
	}
	ref<Shader> ShaderLib::get(const std::string& name)
	{
		ASSERT(exists(name), "No shader with name '{0}' found", name);
		return _shaders[name];
	}
}