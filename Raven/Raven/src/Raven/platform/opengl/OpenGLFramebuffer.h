#pragma once
#include <Raven/rendering/Framebuffer.h>

namespace rvn {

	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void invalidate();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void resize(std::uint32_t width, std::uint32_t height) override;

		virtual const FramebufferSpecification& getSpecification() const override;

		virtual std::uint32_t getColorAttachmentID() const override { return _colorAttachment; }
	private:
		std::uint32_t _id = 0;
		std::uint32_t _colorAttachment = 0, _depthAttachment = 0; // Color / depth part of framebuffer
		FramebufferSpecification _spec;
	};
}