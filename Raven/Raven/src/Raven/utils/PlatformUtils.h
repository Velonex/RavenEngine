#pragma once

#include <string>
#include <optional>

namespace rvn {

	class FileDialogs {
	public:
		static std::optional<std::string> openFile(const char* filter);
		static std::optional<std::string> saveFile(const char* filter);
	};
}