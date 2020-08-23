#include <pch.h>
#include "Input.h"

#ifdef PLATFORM_WINDOWS
	#include <Raven/platform/windows/WindowsInput.h>
#endif

namespace rvn {
	Input* Input::createInput(Application* appPtr)
	{
#ifdef PLATFORM_WINDOWS
		return new WindowsInput(appPtr);
#elif
		ASSERT(false, "Unknown platform");
		return nullptr;
#endif
	}
}