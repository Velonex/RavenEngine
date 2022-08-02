#pragma once
#include <Raven_Core.h>

namespace rvn {

	enum class AttachmentLayout {
		None,

		// Color
		RGBA8,
		RGB8,
		RED_UINT,

		// Depth
		DEPTH24STENCIL8
	};

	struct AttachmentSpecification {
		AttachmentLayout layout = AttachmentLayout::None;

	};

	struct FramebufferSpecification {
		std::uint32_t width = 0, height = 0;
		std::vector<AttachmentSpecification> attachments;
		std::uint32_t samples = 1;

		bool swapChainTarget = false;
	};
	class Framebuffer {
	public:
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(std::uint32_t width, std::uint32_t height) = 0;

		virtual const FramebufferSpecification& getSpecification() const = 0;

		virtual std::uint32_t getColorAttachmentID(std::uint32_t index = 0) const = 0;
		virtual std::uint32_t pixelAt(std::uint32_t index, int x, int y) = 0;

		virtual void clearAttachment(std::uint32_t index, std::uint32_t value) = 0;

		static ref<Framebuffer> create(const FramebufferSpecification& spec);
	};
}