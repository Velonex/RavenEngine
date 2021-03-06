#pragma once

namespace rvn {

	class GraphicsContext {
	public:
		virtual void init() = 0;
		virtual void swapBuffers() = 0;
		static GraphicsContext* createGraphicsContext(void* window);
	};

}