#pragma once
#include <Raven/rendering/Shader.h>

namespace rvn {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource, const std::string& name);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual const std::string& getName() const override;
	private:
		void compileShaders(std::unordered_map<std::string, std::string> shaderSources);
	private:
		std::uint32_t _id = 0;
		std::string _name;
	};
}