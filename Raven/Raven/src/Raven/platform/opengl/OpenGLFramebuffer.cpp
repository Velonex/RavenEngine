#include <pch.h>
#include "OpenGLFramebuffer.h"
#include <glad/glad.h>

namespace rvn {

	static const std::uint32_t s_maxFramebufferSize = 8192;

	static const GLuint attachmentLayoutToInternalFormat(AttachmentLayout layout) {
		if (layout == AttachmentLayout::RGBA8) return GL_RGBA8;
		else if (layout == AttachmentLayout::RGB8) return GL_RGB8;
		else if (layout == AttachmentLayout::RED_UINT) return GL_R32UI;
		else if (layout == AttachmentLayout::DEPTH24STENCIL8) return GL_DEPTH24_STENCIL8;
		ASSERT(false, "Unknown attachment layout");
		return 0;
	}
	static const GLuint attachmentLayoutToFormat(AttachmentLayout layout) {
		if (layout == AttachmentLayout::RGBA8) return GL_RGBA;
		else if (layout == AttachmentLayout::RGB8) return GL_RGB;
		else if (layout == AttachmentLayout::RED_UINT) return GL_RED_INTEGER;
		else if (layout == AttachmentLayout::DEPTH24STENCIL8) return GL_DEPTH_STENCIL;
		ASSERT(false, "Unknown attachment layout");
		return 0;
	}


	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: _spec(spec)
	{
		// Copy the attachments into the respective vectors;
		for (const auto& attachment : spec.attachments) {
			switch (attachment.layout) {
			case AttachmentLayout::RGBA8:
			case AttachmentLayout::RGB8:
			case AttachmentLayout::RED_UINT:
				_colorSpecs.push_back(attachment);
				break;
			case AttachmentLayout::DEPTH24STENCIL8:
				_depthSpecs.push_back(attachment);
				break;
			default:
				ASSERT(false, "Unknown attachment layout");
			}
		}

		invalidate();
	}
	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &_id);
		glDeleteTextures((GLsizei)_colorAttachments.size(), _colorAttachments.data());
		glDeleteTextures((GLsizei)_depthAttachments.size(), _depthAttachments.data());
	}
	void OpenGLFramebuffer::invalidate()
	{
		if (_id) {
			glDeleteFramebuffers(1, &_id);
			glDeleteTextures((GLsizei)_colorAttachments.size(), _colorAttachments.data());
			glDeleteTextures((GLsizei)_depthAttachments.size(), _depthAttachments.data());
		}

		glCreateFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);


		// Create color attachments
		_colorAttachments.resize(_colorSpecs.size());
		if(_colorSpecs.size() >= 0) glCreateTextures(GL_TEXTURE_2D, (GLsizei)_colorSpecs.size(), &_colorAttachments[0]);
		for (int i = 0; i < _colorSpecs.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _colorAttachments[i]);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				attachmentLayoutToInternalFormat(_colorSpecs[i].layout),
				_spec.width,
				_spec.height,
				0,
				attachmentLayoutToFormat(_colorSpecs[i].layout),
				GL_UNSIGNED_BYTE,
				nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _colorAttachments[i], 0);
		}

		_depthAttachments.resize(_depthSpecs.size());
		if(_depthSpecs.size() > 0) glCreateTextures(GL_TEXTURE_2D, (GLsizei)_depthSpecs.size(), &_depthAttachments[0]);
		for (int i = 0; i < _depthSpecs.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _depthAttachments[i]);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _spec.width, _spec.height);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depthAttachments[i], 0);
		}

		ASSERT(_colorSpecs.size() < 5, "The framebuffer can only draw to 4 color attachments yet");
		const GLenum buffers[] { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers((GLsizei)_colorSpecs.size(), buffers);

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
	std::uint32_t OpenGLFramebuffer::getColorAttachmentID(std::uint32_t index) const
	{
		ASSERT(index < _colorSpecs.size(), "Attachment index out of range");
		return _colorAttachments[index];
	}
	std::uint32_t OpenGLFramebuffer::pixelAt(std::uint32_t index, int x, int y)
	{
		ASSERT(index < _colorSpecs.size(), "Attachment index out of range");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
		std::uint32_t data;
		glReadPixels(x, _spec.height - (y + 1), 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &data);
		return data;
	}
	void OpenGLFramebuffer::clearAttachment(std::uint32_t index, std::uint32_t value)
	{
		ASSERT(index < _colorSpecs.size(), "Attachment index out of range");
		glClearTexImage(_colorAttachments[index], 0, attachmentLayoutToFormat(_colorSpecs[index].layout), GL_UNSIGNED_INT, &value);
	}
}