#pragma once

namespace rvn {
	enum Configuration {
		Debug, Release, Distribution
	};

#ifdef DEBUG
	static constexpr Configuration configuration = Configuration::Debug;
#endif // DEBUG
#ifdef RELEASE
	static constexpr Configuration configuration = Configuration::Release;
#endif // RELEASE
#ifdef DIST
	static constexpr Configuration configuration = Configuration::Distribution;
#endif // DIST
}