#pragma once

#include <cstdint>

namespace rvn {

	// Helper struct that enables the converter to return multiple bytes from a function
	template<std::uint8_t size>
	struct bytes {
		bytes() = default;
		bytes(const std::string& str) {
			memcpy_s(&chars, size, &str[0], str.length());
		}

		char operator[](size_t i) {
			return chars[i];
		}

		char chars[size] = { 0 };
	};

	struct Converter {
		template<typename T>
		static bytes<sizeof(T)> toBytes(T val) { static_assert(false); }

		template<>
		static bytes<sizeof(float)> toBytes(float val) {
			std::uint32_t asInt = 0;
			memcpy_s(&asInt, sizeof(asInt), &val, sizeof(val));
			bytes<sizeof(float)> ret;
			for (int i = 0; i < sizeof(float); i++) {
				ret.chars[i] = (asInt >> 8 * i) & 0xFF;
			}
			return ret;
		}
		template<>
		static bytes<sizeof(std::uint16_t)> toBytes(std::uint16_t val) {
			bytes<sizeof(std::uint16_t)> ret;
			for (int i = 0; i < sizeof(std::uint16_t); i++) {
				ret.chars[i] = (val >> 8 * i) & 0xFF;
			}
			return ret;
		}
		template<>
		static bytes<sizeof(std::uint32_t)> toBytes(std::uint32_t val) {
			bytes<sizeof(std::uint32_t)> ret;
			for (int i = 0; i < sizeof(std::uint32_t); i++) {
				ret.chars[i] = (val >> 8 * i) & 0xFF;
			}
			return ret;
		}
		template<>
		static bytes<sizeof(std::uint64_t)> toBytes(std::uint64_t val) {
			bytes<sizeof(std::uint64_t)> ret;
			for (int i = 0; i < sizeof(std::uint64_t); i++) {
				ret.chars[i] = (val >> 8 * i) & 0xFF;
			}
			return ret;
		}
		template<typename T>
		static T toNum(bytes<sizeof(T)> val) { static_assert(false); }

		template<>
		static float toNum(bytes<sizeof(float)> val) {
			std::uint32_t asInt = 0;
			for (int i = 0; i < sizeof(float); i++) {
				asInt |= std::uint32_t((std::uint8_t)val[i]) << (i * 8);
			}
			float ret = 0;
			memcpy_s(&ret, sizeof(ret), &asInt, sizeof(asInt));
			return ret;
		}
		template<>
		static std::uint16_t toNum(bytes<sizeof(std::uint16_t)> val) {
			std::uint16_t ret = 0;
			for (int i = 0; i < sizeof(std::uint16_t); i++) {
				ret |= std::uint16_t((std::uint8_t)val[i]) << (i * 8);
			}
			return ret;
		}
		template<>
		static std::uint32_t toNum(bytes<sizeof(std::uint32_t)> val) {
			std::uint32_t ret = 0;
			for (int i = 0; i < sizeof(std::uint32_t); i++) {
				ret |= std::uint32_t((std::uint8_t)val[i]) << (i * 8);
			}
			return ret;
		}
		template<>
		static std::uint64_t toNum(bytes<sizeof(std::uint64_t)> val) {
			std::uint64_t ret = 0;
			for (int i = 0; i < sizeof(std::uint64_t); i++) {
				ret |= std::uint64_t((std::uint8_t)val[i]) << (i * 8);
			}
			return ret;
		}
	};
	// TODO: maybe add byte to vec conversions

}