#pragma once
#include <Raven/rendering/RendererAPI.h>

namespace rvn {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void init() override;

		virtual void setClearColor(const glm::vec4& color) override;
		virtual void clear() override;

		virtual void draw(const ref<VertexArray> vertexArray) override;

		virtual void setViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) override;
	private:
		static void OpenGLRendererAPI::OpenGLMessageCallback(unsigned source,
			unsigned type,
			unsigned id,
			unsigned severity,
			int length,
			const char* message,
			const void* userParam);
	};

}