#include <pch.h>
#include "Window.h"

#ifdef PLATFORM_WINDOWS
	#include "Raven/platform/windows/WindowsWindow.h"
#endif

namespace rvn {

	Window* Window::createWindow(const WindowProps& props, Application* appPtr)
	{
#ifdef PLATFORM_WINDOWS
		return new WindowsWindow(props, appPtr);
#elif
		ASSERT(false, "Unknown platform");
		return nullptr;
#endif
	}

}