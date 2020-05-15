#pragma once
#include <Raven/rendering/RendererAPI.h>

namespace rvn {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void init() override;

		virtual void setClearColor(const glm::vec4& color) override;
		virtual void clear() override;
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