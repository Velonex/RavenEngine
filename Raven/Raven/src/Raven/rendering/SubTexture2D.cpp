#include <pch.h>
#include "SubTexture2D.h"

namespace rvn {

	SubTexture2D::SubTexture2D(const ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: _texture(texture)
	{
		_texCoords[0] = { min.x, min.y };
		_texCoords[1] = { max.x, min.y };
		_texCoords[2] = { max.x, max.y };
		_texCoords[3] = { min.x, max.y };
	}
	ref<SubTexture2D> SubTexture2D::createFromCoords(const ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& tileSize)
	{
		return createRef<SubTexture2D>(texture,
			// min
			glm::vec2( (coords.x * tileSize.x) / texture->getWidth(), (coords.y * tileSize.y) / texture->getHeight()),
			// max
			glm::vec2( ((coords.x + 1) * tileSize.x) / texture->getWidth(), ((coords.y + 1) * tileSize.y) / texture->getHeight()));
	}
}