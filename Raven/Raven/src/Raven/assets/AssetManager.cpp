#include <pch.h>
#include "AssetManager.h"
#include "IdLookup.h"
#include <fstream>

namespace rvn {

	std::unordered_map<std::uint64_t, std::weak_ptr<Texture2D>> AssetManager::s_textureCache;

	ref<Texture2D> AssetManager::loadTexture(std::uint64_t id)
	{
		eraseExpired<Texture2D>(s_textureCache);
		if (s_textureCache.find(id) != s_textureCache.end()) {
			return s_textureCache[id].lock();
		}
		else {
			std::ifstream in(IdLookup::getPath(id), std::ios::binary | std::ios::ate);
			if (in) {
				std::uint64_t length = in.tellg();
				in.seekg(0);
				ref<std::string> file = createRef<std::string>();
				file->resize(length);
				in.read(&(*file)[0], length);
				auto sp = s_textureCache[id].lock();
				if (!sp) s_textureCache[id] = sp = Texture2D::create(file);
				return sp;
			}
			else {
				ASSERT(false, "Couldn't open file");
			}
		}
		return ref<Texture2D>();
	}

	template<typename T>
	void AssetManager::eraseExpired(std::unordered_map<std::uint64_t, std::weak_ptr<T>>& map)
	{
		for (auto it = map.begin(); it != map.end();) {
			if ((*it).second.expired())
				it = map.erase(it);
			else
				it++;
		}
	}
}