#include <pch.h>
#include "VertexArray.h"
#include <Raven/platform/opengl/OpenGLVertexArray.h>

namespace rvn {
	ref<VertexArray> VertexArray::create()
	{
		switch (RendererAPI::getRendererApi()) {
		case RendererAPI::API::OpenGL: 
			return createRef<OpenGLVertexArray>();
		}
		ASSERT(false, "Unsupported RendererAPI");
		return nullptr;
	}
}