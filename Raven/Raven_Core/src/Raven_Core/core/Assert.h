#pragma once
#include <Raven_Core/core/Logger.h>
#include "DebugBreak.h"
#include <string>

#if defined(DEBUG)
	#define ENABLE_ASSERTS
#endif
#ifdef ENABLE_ASSERTS
#define ASSERT(x, basestr,...) { if (!(x)) {\
		std::string str = "Assertion failed: ";\
		str.append(basestr);\
		rvn::Logger::getEngineLogger()->error(str.c_str(), __VA_ARGS__);\
		RVN_DEBUGBREAK();}}
#else
	#define ASSERT(x, ...)
#endif