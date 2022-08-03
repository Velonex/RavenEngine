#pragma once
#include <string>
#include <filesystem>

namespace rvn {

	class Shader {
	public:
		virtual ~Shader() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const std::string& getName() const = 0;

		static ref<Shader> create(const std::string& vertexShaderSource, const std::string& fragmentShaderSource, const std::string& name);
		static ref<Shader> create(const std::filesystem::path& path);
		
		virtual void setInt(const std::string& name, int value) const = 0;
		virtual void setIntArray(const std::string& name, int* values, uint32_t count) const = 0;

		virtual void setFloat(const std::string& name, float value) const = 0;
		virtual void setFloat2(const std::string& name, const glm::vec2& value) const = 0;
		virtual void setFloat3(const std::string& name, const glm::vec3& value) const = 0;
		virtual void setFloat4(const std::string& name, const glm::vec4& value) const = 0;

		virtual void setMat3(const std::string& name, const glm::mat3& value) const = 0;
		virtual void setMat4(const std::string& name, const glm::mat4& value) const = 0;
	};

	class ShaderLib {
	public:
		void add(ref<Shader> shader);
		void add(const std::string& name, ref<Shader> shader);

		//ref<Shader> load(const std::string& name, const std::string& filepath);
		//ref<Shader> load(const std::string& filepath);

		bool exists(const std::string& name) const;

		ref<Shader> get(const std::string& name);
	private:
		std::unordered_map<std::string, ref<Shader>> _shaders;
	};
}