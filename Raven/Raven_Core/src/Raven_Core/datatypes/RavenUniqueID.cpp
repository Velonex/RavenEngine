#include "RavenUniqueID.h"

#include <random>
#include <chrono>

namespace rvn {

	static std::random_device rd;
	static std::mt19937 mt(rd());
	static std::uniform_int_distribution<int> dist(0, 255);
	static int counter = 0;

	RUID RUID::generateRUID() {
		counter++;
		if (counter > 255) counter = 0;
		auto now = std::chrono::system_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm tmPoint;
		gmtime_s(&tmPoint, &t);
		std::uint64_t ret = (
			(static_cast<std::uint64_t>(dist(mt)) << 0) & 0xFF // RNG
			| (static_cast<std::uint64_t>(counter) << 8) & 0xFF00 // counter
			| (static_cast<std::uint64_t>(t % 1000) << 16) & 0x3FF0000 // millis
			| (static_cast<std::uint64_t>(tmPoint.tm_sec) << 26) & 0xFC000000 // second
			| (static_cast<std::uint64_t>(tmPoint.tm_min) << 32) & 0x3F00000000 // minute
			| (static_cast<std::uint64_t>(tmPoint.tm_hour) << 37) & 0x3E000000000 // hour
			| (static_cast<std::uint64_t>(tmPoint.tm_mday) << 42) & 0x7C0000000000 // day
			| (static_cast<std::uint64_t>(tmPoint.tm_mon) << 46) & 0x3C00000000000 // month
			| (static_cast<std::uint64_t>(tmPoint.tm_year) << 52) & 0xFFF0000000000000 // year
			);
		return RUID(ret);
	}
}