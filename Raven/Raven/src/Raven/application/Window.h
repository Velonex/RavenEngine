#pragma once
#include <string>
#include <Raven_Core/event/EventSystem.h>
//#include <Raven/rendering/GraphicsContext.h>

namespace rvn {
	class Application;
	struct WindowProps {
		typedef unsigned int uint;
		uint width, height;
		std::string title;
		WindowProps(uint width = 1280, uint height = 720, std::string title = "My Window") : width(width), height(height), title(title) { }
	};
	class Window {
	public:
		static Window* createWindow(const WindowProps& props, Application* appPtr);

		virtual void onUpdate() = 0;

		virtual ~Window() {}

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool getVSync() const = 0;
		inline virtual void* getNativeWindow() const = 0;
		//virtual ref<GraphicsContext> getGraphicsContext() const = 0;
	};
}