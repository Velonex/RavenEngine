#include <pch.h>
#include "AssetManager.h"
#include "IdLookup.h"
#include <fstream>

namespace rvn {

	std::unordered_map<std::uint64_t, std::weak_ptr<Texture2D>> AssetManager::s_textureCache;
	std::unordered_map<std::uint64_t, std::weak_ptr<Mesh>> AssetManager::s_meshCache;

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
				// Set white texture instead
				auto sp = s_textureCache[id].lock();
				if (!sp) s_textureCache[id] = sp = Texture2D::create(1, 1);
				std::uint32_t whiteData = 0xFFFFFFFF;
				sp->setData(&whiteData, 4);
				return sp;
			}
		}
		return ref<Texture2D>();
	}

	ref<Mesh> AssetManager::loadMesh(std::uint64_t id)
	{
		// TODO: alternative loading that doesn't rely on other function to future proof it for packed assets
		eraseExpired<Mesh>(s_meshCache);
		if (s_meshCache.find(id) != s_meshCache.end()) {
			return s_meshCache[id].lock();
		}
		else {
			auto sp = s_meshCache[id].lock();
			if (!sp) s_meshCache[id] = sp = Mesh::createFromFile(IdLookup::getPath(id), MeshFormat::OBJ);
			return sp;
		}
		return ref<Mesh>();
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