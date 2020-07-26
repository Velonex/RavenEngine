#pragma once
#include <Raven/rendering/Texture.h>

namespace rvn {

	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(std::uint32_t width, std::uint32_t height);
		virtual ~OpenGLTexture2D();

		virtual std::uint32_t getWidth() const override { return _width; }
		virtual std::uint32_t getHeight() const override { return _height; }

		virtual void setData(void* data, std::uint32_t size) override;

		virtual void bind(std::uint32_t slot = 0) const override;
	private:
		std::uint32_t _width, _height;
		std::uint32_t _id;
		std::string _path;
	};

}