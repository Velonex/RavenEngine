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

		virtual std::uint32_t getColorAttachmentID(std::uint32_t index) const override;
		virtual std::uint32_t pixelAt(std::uint32_t index, int x, int y) override;

		virtual void clearAttachment(std::uint32_t index, std::uint32_t value) override;
	private:
		std::uint32_t _id = 0;
		FramebufferSpecification _spec;

		std::vector<AttachmentSpecification> _colorSpecs;
		std::vector<AttachmentSpecification> _depthSpecs;

		std::vector<uint32_t> _colorAttachments;
		std::vector<uint32_t> _depthAttachments;
	};
}