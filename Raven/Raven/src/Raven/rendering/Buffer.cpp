#include <pch.h>
#include "Buffer.h"
#include <Raven/platform/opengl/OpenGLBuffer.h>

namespace rvn {
	ref<VertexBuffer> VertexBuffer::create(void* vertices, uint32_t size)
	{
		switch (RendererAPI::getRendererApi()) {
		case RendererAPI::API::OpenGL:
			return createRef<OpenGLVertexBuffer>(vertices, size);
		}
		ASSERT(false, "Unsupported RendererAPI");
		return nullptr;
	}
	ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t count)
	{
		switch (RendererAPI::getRendererApi()) {
		case RendererAPI::API::OpenGL:
			return createRef<OpenGLIndexBuffer>(indices, count);
		}
		ASSERT(false, "Unsupported RendererAPI");
		return nullptr;
	}
}