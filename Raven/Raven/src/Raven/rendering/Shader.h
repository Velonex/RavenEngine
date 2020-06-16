#pragma once
#include <string>

namespace rvn {

	class Shader {
	public:
		virtual ~Shader() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const std::string& getName() const = 0;

		static ref<Shader> create(const std::string& vertexShaderSource, const std::string& fragmentShaderSource, const std::string& name);
		//TODO: Create Shader from file
		
		virtual void setInt(const std::string& name, int value) const = 0;

		virtual void setFloat(const std::string& name, float value) const = 0;
		virtual void setFloat2(const std::string& name, const glm::vec2& value) const = 0;
		virtual void setFloat3(const std::string& name, const glm::vec3& value) const = 0;
		virtual void setFloat4(const std::string& name, const glm::vec4& value) const = 0;

		virtual void setMat3(const std::string& name, const glm::mat3& value) const = 0;
		virtual void setMat4(const std::string& name, const glm::mat4& value) const = 0;
	};
	//TODO: Shader lib
}