#include <pch.h>
#include "OpenGLTexture.h"

#include <Raven_Core/core/Assert.h>
#include <stb_image.h>
#include <glad/glad.h>

namespace rvn {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: _path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		ASSERT(data, "Failed to load image");
		_width = width;
		_height = height;

		GLenum internalFormat = 0, format = 0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			format = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			format = GL_RGB;
		}
		else {
			ASSERT(false, "Unsupported channel count of {0}", channels);
		}
		glCreateTextures(GL_TEXTURE_2D, 1, &_id);
		glTextureStorage2D(_id, 1, internalFormat, width, height);

		glTextureParameteri(_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(_id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

	}
	OpenGLTexture2D::OpenGLTexture2D(std::uint32_t width, std::uint32_t height)
		: _width(width), _height(height), _path("")
	{
		GLenum internalFormat = GL_RGBA8, format = GL_RGBA;
		
		glCreateTextures(GL_TEXTURE_2D, 1, &_id);
		glTextureStorage2D(_id, 1, internalFormat, width, height);

		glTextureParameteri(_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &_id);
	}
	void OpenGLTexture2D::setData(void* data, std::uint32_t size)
	{
		int bpp = 4; // TODO: make automatic if the data format is changeable
		ASSERT(size == _width * _height * bpp, "Data must fill entire texture");
		glTextureSubImage2D(_id, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	void OpenGLTexture2D::bind(std::uint32_t slot) const
	{
		glBindTextureUnit(slot, _id);
	}
}