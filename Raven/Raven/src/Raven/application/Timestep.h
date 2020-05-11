#pragma once

namespace rvn {

	class Timestep {
	public:
		Timestep(float time = 0.0f) : _time(time) {

		}
		float getSeconds() const { return _time; }
		float getMilliseconds() const { return _time * 1000.f; }
		float operator*(float rhs) {
			return _time * rhs;
		}
		float operator*=(float rhs) {
			return _time * rhs;
		}
		operator float() {
			return _time;
		}
	private:
		float _time = 0.0f;
	};

}