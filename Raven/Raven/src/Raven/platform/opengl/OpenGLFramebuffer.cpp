#include <pch.h>
#include "OpenGLFramebuffer.h"
#include <glad/glad.h>

namespace rvn {

	static const std::uint32_t s_maxFramebufferSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: _spec(spec)
	{
		invalidate();
	}
	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &_id);
		glDeleteTextures(1, &_colorAttachment);
		glDeleteTextures(1, &_depthAttachment);
	}
	void OpenGLFramebuffer::invalidate()
	{
		if (_id) {
			glDeleteFramebuffers(1, &_id);
			glDeleteTextures(1, &_colorAttachment);
			glDeleteTextures(1, &_depthAttachment);
		}

		glCreateFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);

		glCreateTextures(GL_TEXTURE_2D, 1, &_colorAttachment);
		glBindTexture(GL_TEXTURE_2D, _colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _spec.width, _spec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &_depthAttachment);
		glBindTexture(GL_TEXTURE_2D, _depthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _spec.width, _spec.height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depthAttachment, 0);

		ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFramebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
		glViewport(0, 0, _spec.width, _spec.height);
	}
	void OpenGLFramebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFramebuffer::resize(std::uint32_t width, std::uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_maxFramebufferSize || height > s_maxFramebufferSize) {
			LOG_ENGINE_WARN("Tried to resize framebuffer to invalid size of: [{0}, {1}]", width, height);
			return;
		}
		_spec.width = width;
		_spec.height = height;

		invalidate();
	}
	const FramebufferSpecification& OpenGLFramebuffer::getSpecification() const
	{
		return _spec;
	}
}