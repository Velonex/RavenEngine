#pragma once
#include <cstdint>
#include <string>
#include <iomanip>
#include <sstream>

namespace rvn {
	struct RUID {
	public:
		RUID() = default;
		RUID(std::uint64_t ruid) : ruid(ruid) {}

		std::uint64_t ruid = 0x0000000000000000;

		static RUID generateRUID();
	public:
		operator std::uint64_t() const {
			return ruid;
		}
	};
}