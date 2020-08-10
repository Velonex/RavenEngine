#include <pch.h>
#include "OpenGLShader.h"
#include <glad/glad.h>
#include <gtc/type_ptr.hpp>

namespace rvn {
	OpenGLShader::OpenGLShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource, const std::string& name)
		: _name(name)
	{
		std::unordered_map<std::string, std::string> sources;
		sources["vert"] = vertexShaderSource;
		sources["frag"] = fragmentShaderSource;
		compileShaders(sources);
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(_id);
	}
	void OpenGLShader::bind() const
	{
		glUseProgram(_id);
	}
	void OpenGLShader::unbind() const
	{
		glUseProgram(0);
	}
	const std::string& OpenGLShader::getName() const
	{
		return _name;
	}
	void OpenGLShader::uploadUniformInt(const std::string& name, int value) const
	{
		GLuint loc = glGetUniformLocation(_id, name.c_str());
		glUniform1i(loc, value);
	}
	void OpenGLShader::uploadUniformIntArray(const std::string& name, int* values, uint32_t count) const
	{
		GLuint loc = glGetUniformLocation(_id, name.c_str());
		glUniform1iv(loc, count, values);
	}
	void OpenGLShader::uploadUniformFloat(const std::string& name, float value) const
	{
		GLuint loc = glGetUniformLocation(_id, name.c_str());
		glUniform1f(loc, value);
	}
	void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& value) const
	{
		GLuint loc = glGetUniformLocation(_id, name.c_str());
		glUniform2f(loc, value.x, value.y);
	}
	void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& value) const
	{
		GLuint loc = glGetUniformLocation(_id, name.c_str());
		glUniform3f(loc, value.x, value.y, value.z);
	}
	void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& value) const
	{
		GLuint loc = glGetUniformLocation(_id, name.c_str());
		glUniform4f(loc, value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& value) const
	{
		GLuint loc = glGetUniformLocation(_id, name.c_str());
		glUniformMatrix3fv(loc, 1, false, glm::value_ptr(value));
	}
	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& value) const
	{
		GLuint loc = glGetUniformLocation(_id, name.c_str());
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(value));
	}
	void OpenGLShader::setInt(const std::string& name, int value) const
	{
		uploadUniformInt(name, value);
	}
	void OpenGLShader::setIntArray(const std::string& name, int* values, uint32_t count) const
	{
		uploadUniformIntArray(name, values, count);
	}
	void OpenGLShader::setFloat(const std::string& name, float value) const
	{
		uploadUniformFloat(name, value);
	}
	void OpenGLShader::setFloat2(const std::string& name, const glm::vec2& value) const
	{
		uploadUniformFloat2(name, value);
	}
	void OpenGLShader::setFloat3(const std::string& name, const glm::vec3& value) const
	{
		uploadUniformFloat3(name, value);
	}
	void OpenGLShader::setFloat4(const std::string& name, const glm::vec4& value) const
	{
		uploadUniformFloat4(name, value);
	}
	void OpenGLShader::setMat3(const std::string& name, const glm::mat3& value) const
	{
		uploadUniformMat3(name, value);
	}
	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& value) const
	{
		uploadUniformMat4(name, value);
	}
	GLenum stringToShaderType(const std::string& str) {
		if (str == "vert") return GL_VERTEX_SHADER;
		if (str == "frag" || str == "pixel") return GL_FRAGMENT_SHADER;
		ASSERT(false, "Unknown/Unsupported shader type");
		return 0;
	}
	void OpenGLShader::compileShaders(std::unordered_map<std::string, std::string> shaderSources)
	{
		std::unordered_map<GLenum, std::string> sources;
		for (auto s : shaderSources) {
			sources[stringToShaderType(s.first)] = s.second;
		}
		GLuint* shaderIDs = new GLuint[sources.size()];

		std::uint32_t counter = 0;
		for (auto s : sources) {
			shaderIDs[counter] = glCreateShader(s.first);
			const GLchar* shaderSource = s.second.c_str();
			glShaderSource(shaderIDs[counter], 1, &shaderSource, 0);

			glCompileShader(shaderIDs[counter]);

			GLint compiled = 0;
			glGetShaderiv(shaderIDs[counter], GL_COMPILE_STATUS, &compiled);
			if (compiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shaderIDs[counter], GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shaderIDs[counter], maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shaderIDs[counter]);

				LOG_ENGINE_ERROR("{0}", infoLog.data());
				ASSERT(false, "Shader compilation failed!");
				delete[] shaderIDs;
				return;
			}
			counter++;
		}

		_id = glCreateProgram();
		for (GLuint i = 0; i < counter; i++) {
			glAttachShader(_id, shaderIDs[i]);
		}
		glLinkProgram(_id);

		GLint linked = 0;
		glGetProgramiv(_id, GL_LINK_STATUS, &linked);
		if (linked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(_id, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(_id);

			LOG_ENGINE_ERROR("{0}", infoLog.data());
			ASSERT(false, "Shader program linkage failed!");
			delete[] shaderIDs;
			return;
		}
		for (GLuint i = 0; i < counter; i++) {
			glDetachShader(_id, shaderIDs[i]);
			glDeleteShader(shaderIDs[i]);
		}
		delete[] shaderIDs;
	}
}