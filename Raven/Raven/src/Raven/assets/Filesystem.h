#pragma once
#include <optional>
#include <filesystem>

namespace rvn {

	class Filesystem {
	public:
		static void init(const std::string& base);
		static const std::optional<ref<std::string>> readFile(const std::string& path);
		static bool exists(const std::string& path);
		static std::string getBase() { return _base.string(); }
	private:
		static std::filesystem::path _base;
	};

}