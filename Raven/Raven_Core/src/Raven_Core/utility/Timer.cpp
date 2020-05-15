#include "Timer.h"
#include <Raven_Core/core/Assert.h>

namespace rvn {
	void Timer::startTimer()
	{
		ASSERT(_startMillis == 0, "The timer has to be reset before it can be started again");
		_startMillis = std::chrono::duration_cast<Timer::_milliseconds>(_clock::now().time_since_epoch()).count();
	}

	void Timer::stopTimer()
	{
		ASSERT(_startMillis != 0, "The timer has to be started before it can be stopped");
		ASSERT(_endMillis == 0, "The timer has to be reset before it can be stopped again");
		_endMillis = std::chrono::duration_cast<Timer::_milliseconds>(_clock::now().time_since_epoch()).count();
	}

	uint64_t Timer::getDuration()
	{
		ASSERT(_startMillis != 0, "The timer has to be started before the duration can be calculated");
		ASSERT(_endMillis != 0, "The timer has to be stopped before the duration can be calculated");
		return _endMillis - _startMillis;
	}

	bool Timer::hasReached(uint64_t milliseconds)
	{
		if (_startMillis == 0) startTimer();
		if ((std::chrono::duration_cast<Timer::_milliseconds>(_clock::now().time_since_epoch()).count() - _startMillis) >= milliseconds)
		{
			reset();
			return true;
		}
		return false;
	}


	void Timer::reset()
	{
		_startMillis = 0;
		_endMillis = 0;
	}

}