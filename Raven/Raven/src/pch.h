#pragma once

#include <Raven_Core/core/Platform.h>

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <Raven/rendering/RendererAPI.h>
#include <Raven_Core/core/Logger.h>
#include <Raven_Core/core/Assert.h>
#include <Raven_Core/core/Platform.h>

#if defined(PLATFORM_WINDOWS) 
	#include <Windows.h>
#endif