#pragma once
#include <memory>

namespace rvn {

	template<typename T>
	using ref = std::shared_ptr<T>;
	template<typename T, typename... Args>
	constexpr ref<T> createRef(Args&&... args) {
		return std::make_shared(std::forward<Args>(args)...);
	}

	template<typename T>
	using scope = std::unique_ptr<T>;
	template<typename T, typename... Args>
	constexpr scope<T> createScope(Args&&... args) {
		return std::make_unique(std::forward<Args>(args)...);
	}

}