#pragma once
#include <Raven_Core.h>

namespace rvn {

	struct FramebufferSpecification {
		std::uint32_t width = 0, height = 0;
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

		virtual std::uint32_t getColorAttachmentID() const = 0;

		static ref<Framebuffer> create(const FramebufferSpecification& spec);
	};
}