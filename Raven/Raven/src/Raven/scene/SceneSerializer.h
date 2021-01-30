#pragma once

#include <string>
#include "Scene.h"

namespace rvn {

	class SceneSerializer {
	public:
		static void serialize(const ref<Scene>& scene, const std::string& filepath);
		static void serializeRuntime(const ref<Scene>& scene, const std::string& filepath);

		static bool deserialize(const ref<Scene>& scene, const std::string& filepath);
		static void deserializeRuntime(const ref<Scene>& scene, const std::string& filepath);
	private:
		static const std::string RUNTIME_SCENEFILE_MAGIC_NUMBER;
	};

}