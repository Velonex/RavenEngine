#pragma once
#include <Raven/rendering/Texture.h>
#include <Raven_Core/datatypes/RavenUniqueID.h>
#include <any>
#include <optional>

namespace rvn {

	class AssetManager {
	public:
		static ref<Texture2D> loadTexture(std::uint64_t id);
	private:
		template<typename T>
		static void eraseExpired(std::unordered_map<std::uint64_t, std::weak_ptr<T>>& map);
	private:
		static std::unordered_map<std::uint64_t, std::weak_ptr<Texture2D>> s_textureCache;
	};

}