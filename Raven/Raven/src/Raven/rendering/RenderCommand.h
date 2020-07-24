#pragma once
#include "RendererAPI.h"
#include <Raven/rendering/VertexArray.h>

namespace rvn {

	class RenderCommand {
	public:
		static void init() { s_API->init(); }

		static void clear() { s_API->clear(); }
		static void setClearColor(const glm::vec4& color) { s_API->setClearColor(color); }
		
		static void draw(const ref<VertexArray>& vertexArray) { s_API->draw(vertexArray); }

		static void setViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) { s_API->setViewport(x, y, width, height); }
	private:
		static scope<RendererAPI> s_API;
	};

}