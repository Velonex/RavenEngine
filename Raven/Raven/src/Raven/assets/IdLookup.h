#pragma once
#include <Raven_Core/datatypes/RavenUniqueID.h>

namespace rvn {

	class IdLookup {
	public:
		static void loadLookup(const std::string& base);
		static void saveLookup();
		static std::uint64_t getID(const std::string& path);
		static std::string getPath(std::uint64_t id);
	private:
		static std::unordered_map<std::uint64_t, std::string> _lookup;
		static std::string _base;
	};

}