#include <pch.h>
#include "IdLookup.h"
#include <fstream>
#include <Raven_Core/utility/Split.h>
#include <Raven_Core/datatypes/RavenUniqueID.h>

namespace rvn {
	std::unordered_map<std::uint64_t, std::filesystem::path> IdLookup::_lookup = std::unordered_map<std::uint64_t, std::filesystem::path>();
	std::filesystem::path IdLookup::_base = "";

	void IdLookup::loadLookup(const std::string& base)
	{
		_base = base;
		std::ifstream in(base / std::filesystem::path("assets"));
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
			std::ofstream out(_base / std::filesystem::path("assets"));
			if (out) {
				for (auto& entry : _lookup) {
					out << entry.first << ":" << entry.second.string() << std::endl;
				}
			}
			else {
				ASSERT(false, "Couldn't open the asset lookup");
			}
		}
	}
	void IdLookup::saveLookup()
	{
		for (auto it = _lookup.begin(); it != _lookup.end();) {
			if (!std::filesystem::exists(_base / (*it).second))
				it = _lookup.erase(it);
			else
				it++;
		}
		std::ofstream out(_base / std::filesystem::path("assets"));
		if (out) {
			for (auto& entry : _lookup) {
				out << entry.first << ":" << entry.second.string() << std::endl;
			}
		}
		else {
			ASSERT(false, "Couldn't open the asset lookup");
		}
	}
	std::uint64_t IdLookup::getID(const std::string& path)
	{
		std::filesystem::path& base = _base;
		auto it = std::find_if(_lookup.begin(), _lookup.end(), [&, base](std::pair<std::uint64_t, std::filesystem::path>&& p) {
			return p.second == std::filesystem::relative(path, std::filesystem::absolute(base));
			});
		if (it == _lookup.end()) {
			if (std::filesystem::exists(_base / path)) {
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
#ifdef DEBUG
		if (_lookup.find(id) == _lookup.end()) 
			return "ID missing!";
#endif 
		return (_base / _lookup.at(id)).string();
	}
}