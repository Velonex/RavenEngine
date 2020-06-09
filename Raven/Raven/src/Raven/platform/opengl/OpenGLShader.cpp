#include <pch.h>
#include "OpenGLShader.h"
#include <glad/glad.h>

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
	GLenum stringToShaderType(const std::string& str) {
		if (str == "vert") return GL_VERTEX_SHADER;
		if (str == "frag" || str == "pixel") return GL_FRAGMENT_SHADER;
		ASSERT(false, "Unknown/Unsupported shader type");
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