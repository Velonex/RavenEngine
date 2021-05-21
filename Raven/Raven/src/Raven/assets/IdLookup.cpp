#include <pch.h>
#include "IdLookup.h"
#include <fstream>
#include <Raven_Core/utility/Split.h>
#include <Raven_Core/datatypes/RavenUniqueID.h>
#include <filesystem>

namespace rvn {
	std::unordered_map<std::uint64_t, std::string> IdLookup::_lookup = std::unordered_map<std::uint64_t, std::string>();
	std::string IdLookup::_base = "";
	void IdLookup::loadLookup(const std::string& base)
	{
		_base = base + "/";
		std::ifstream in(base + "/assets");
		if (in) {
			std::string line;
			std::vector<std::string> split;
			while (std::getline(in, line)) {
				split = util::split(line, ":");
				if (split.size() != 2) continue;
				try {
					std::uint64_t id = std::stoull(split[0]);
					_lookup[id] = split[1];
				} catch(std::invalid_argument e) {}
			}
		}
		else {
			ASSERT(false, "Couldn't open the asset lookup");
		}
	}
	void IdLookup::saveLookup()
	{
		for (auto& entry : _lookup) {
			if (!std::filesystem::exists(_base + entry.second)) _lookup.erase(entry.first);
		}
		std::ofstream out(_base + "assets");
		if (out) {
			for (auto& entry : _lookup) {
				out << entry.first << ":" << entry.second << std::endl;
			}
		}
		else {
			ASSERT(false, "Couldn't open the asset lookup");
		}
	}
	std::uint64_t IdLookup::getID(const std::string& path)
	{
		std::string& base = _base;
		auto it = std::find_if(_lookup.begin(), _lookup.end(), [&, base](std::pair<std::uint64_t, std::string>&& p) {
			return std::filesystem::path(p.second) == std::filesystem::relative(std::filesystem::path(path), std::filesystem::absolute(std::filesystem::path(base)));
			});
		if (it == _lookup.end()) {
			if (std::filesystem::exists(std::filesystem::path(_base) / std::filesystem::path(path))) {
				std::uint64_t id = RUID::generateRUID();
				_lookup[id] = std::filesystem::relative(path, _base).string();
				return id;
			}
			else {
				return 0;
			}
		}
		return it->first;
	}
	std::string IdLookup::getPath(std::uint64_t id)
	{
		return _base + _lookup.at(id);
	}
}