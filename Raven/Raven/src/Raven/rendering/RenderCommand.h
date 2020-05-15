#pragma once
#include "RendererAPI.h"

namespace rvn {

	class RenderCommand {
	public:
		static void init() { s_API->init(); }

		static void clear() { s_API->clear(); }
		static void setClearColor(const glm::vec4& color) { s_API->setClearColor(color); }
	private:
		static scope<RendererAPI> s_API;
	};

}