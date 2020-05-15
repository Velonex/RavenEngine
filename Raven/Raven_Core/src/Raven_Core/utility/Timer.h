#pragma once
#include <chrono>
#include <cstdint>

namespace rvn {

	class Timer {
	private:
		using _clock = std::chrono::steady_clock;
		using _milliseconds = std::chrono::duration<uint64_t, std::milli>;
		uint64_t _startMillis = 0, _endMillis = 0;
	public:

		void startTimer();
		void stopTimer();
		void reset();

		uint64_t getDuration();
		bool hasReached(uint64_t milliseconds);
	};

}

