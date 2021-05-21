#include <pch.h>
#include "Filesystem.h"
#include <fstream>

namespace rvn {
	std::filesystem::path Filesystem::_base = "";
	void Filesystem::init(const std::string& base)
	{
		_base = base;
	}
	const std::optional<ref<std::string>> Filesystem::readFile(const std::string& path)
	{
		std::filesystem::path p = _base / std::filesystem::path(path);
		std::ifstream in(p, std::ios::binary | std::ios::ate);
		if (in) {
			std::uint64_t length = in.tellg();
			in.seekg(0);
			ref<std::string> ret = createRef<std::string>();
			ret->resize(length);
			in.read(&(*ret)[0], ret->size());
			return ret;
		}
		else {
			LOG_ENGINE_ERROR("Couldn't open file '{0}'", path);
		}
		return std::optional<ref<std::string>>();
	}
	bool Filesystem::exists(const std::string& path)
	{
		return std::filesystem::exists(_base / std::filesystem::path(path));
	}
}