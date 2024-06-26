#pragma once

#include <Raven_Core/core/Platform.h>
#ifdef PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <fstream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <Raven/rendering/RendererAPI.h>
#include <Raven_Core.h>

#if defined(PLATFORM_WINDOWS) 
#include <Windows.h>
#endif