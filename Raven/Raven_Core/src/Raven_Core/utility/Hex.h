#pragma once
#include <string>

namespace rvn {

	class Hex {
	public:
		static std::string toHexUpper(std::uint64_t num);
		static std::string toHexLower(std::uint64_t num);
	};

}