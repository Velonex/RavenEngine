#pragma once
#include <Raven/rendering/Shader.h>
#include <glm.hpp>

namespace rvn {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource, const std::string& name);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual const std::string& getName() const override;

		void uploadUniformInt(const std::string& name, int value) const;

		void uploadUniformFloat(const std::string& name, float value) const;
		void uploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
		void uploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
		void uploadUniformFloat4(const std::string& name, const glm::vec4& value) const;

		void uploadUniformMat3(const std::string& name, const glm::mat3& value) const;
		void uploadUniformMat4(const std::string& name, const glm::mat4& value) const;

		virtual void setInt(const std::string& name, int value) const override;

		virtual void setFloat(const std::string& name, float value) const override;
		virtual void setFloat2(const std::string& name, const glm::vec2& value) const override;
		virtual void setFloat3(const std::string& name, const glm::vec3& value) const override;
		virtual void setFloat4(const std::string& name, const glm::vec4& value) const override;

		virtual void setMat3(const std::string& name, const glm::mat3& value) const override;
		virtual void setMat4(const std::string& name, const glm::mat4& value) const override;
	private:
		void compileShaders(std::unordered_map<std::string, std::string> shaderSources);
	private:
		std::uint32_t _id = 0;
		std::string _name;
	};
}