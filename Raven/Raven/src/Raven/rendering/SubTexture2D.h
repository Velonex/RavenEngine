#pragma once
#include <Raven/rendering/Texture.h>

namespace rvn {

	class SubTexture2D {
	public:
		SubTexture2D(const ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const ref<Texture2D> getTexture() const { return _texture; };
		const glm::vec2* getTexCoords() const { return _texCoords; };

		static ref<SubTexture2D> createFromCoords(const ref<Texture2D>& texture, const glm::vec2& spriteCoords, const glm::vec2& spriteSize);
	private:
		ref<Texture2D> _texture;
		glm::vec2 _texCoords[4];
	};
}