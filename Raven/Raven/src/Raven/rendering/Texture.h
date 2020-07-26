#pragma once

namespace rvn {

	class Texture {
	public:
		virtual ~Texture() {};

		virtual std::uint32_t getWidth() const = 0;
		virtual std::uint32_t getHeight() const = 0;

		virtual void setData(void* data, std::uint32_t size) = 0;

		virtual void bind(std::uint32_t slot = 0) const = 0;
	};
	
	class Texture2D : public Texture {
	public:
		static ref<Texture2D> create(const std::string& path);
		static ref<Texture2D> create(std::uint32_t width, std::uint32_t height);
	};
}