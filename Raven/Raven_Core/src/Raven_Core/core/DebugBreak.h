#pragma once

#if defined(_MSC_VER)
	#define RVN_DEBUGBREAK() __debugbreak()
#else
	#error Raven yet only supports Windows
#endif