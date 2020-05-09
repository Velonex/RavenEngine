#pragma once
#include <Raven_Core/core/Platform.h>

#if defined(PLATFORM_WINDOWS)
#include <Raven/application/Window.h>
#include <GLFW/glfw3.h>

namespace rvn {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props, Application* appPtr);
		~WindowsWindow();

		virtual void onUpdate() override;

		virtual unsigned int getWidth() const override;
		virtual unsigned int getHeight() const override;
		virtual void setVSync(bool enabled) override;
		virtual bool getVSync() const override;
		inline virtual void* getNativeWindow() const override;
	private:
		virtual void _init(const WindowProps& props, Application* appPtr);
		virtual void _shutdown();
	private:
		GLFWwindow* _window;
		//scope<GraphicsContext> _context;
		struct _WindowData {
			unsigned int width, height;
			std::string title;
			bool vsync;
			rvn::EventHandler* handler;
		} _windowData;
	};

}

#endif
