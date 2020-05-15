#pragma once
#include <glm.hpp>
#include <Raven_Core/datatypes/Memory.h>

namespace rvn {

	class RendererAPI {
	public:
		enum class API {
			None = 0, // Not supported
			OpenGL = 1,
			DirectX = 2, // Not supported
			Vulkan = 3 // Not supported
		};
	public:
		virtual void init() = 0;

		virtual void setClearColor(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		static API getRendererApi() { return s_API; };
	private:
		static API s_API;
	};

}