#include <pch.h>
#include "Texture.h"
#include <Raven/platform/opengl/OpenGLTexture.h>

namespace rvn {
	ref<Texture2D> Texture2D::create(const std::string& path)
	{
		switch (RendererAPI::getRendererApi()) {
		case RendererAPI::API::OpenGL:
			return createRef<OpenGLTexture2D>(path);
		}
		ASSERT(false, "Unsupported RendererAPI");
		return nullptr;
	}
	ref<Texture2D> Texture2D::create(std::uint32_t width, std::uint32_t height)
	{
		switch (RendererAPI::getRendererApi()) {
		case RendererAPI::API::OpenGL:
			return createRef<OpenGLTexture2D>(width, height);
		}
		ASSERT(false, "Unsupported RendererAPI");
		return nullptr;
	}
}