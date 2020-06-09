#pragma once
#include <string>

namespace rvn {

	class Shader {
	public:
		virtual ~Shader() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const std::string& getName() const = 0;

		static ref<Shader> createShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource, const std::string& name);
		//TODO: Create Shader from file

		//TODO: Uniforms
	};
	//TODO: Shader lib
}