#pragma once
#include <glm.hpp>
#include <Raven_Core/datatypes/Memory.h>
#include <Raven/rendering/VertexArray.h>

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

		virtual void enableDepthTest() = 0;
		virtual void disableDepthTest() = 0;

		virtual void draw(const ref<VertexArray> vertexArray, std::uint32_t count) = 0;

		virtual void setViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) = 0;

		static API getRendererApi() { return s_API; };
	private:
		static API s_API;
	};

}