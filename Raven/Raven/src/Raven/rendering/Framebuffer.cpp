#include <pch.h>
#include "Framebuffer.h"
#include <Raven/platform/opengl/OpenGLFramebuffer.h>

namespace rvn {
	ref<Framebuffer> Framebuffer::create(const FramebufferSpecification& spec)
	{
		switch (RendererAPI::getRendererApi()) {
		case RendererAPI::API::OpenGL:
			return createRef<OpenGLFramebuffer>(spec);
		}
		ASSERT(false, "Unsupported RendererAPI");
		return nullptr;
	}
}