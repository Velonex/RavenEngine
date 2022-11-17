#pragma once
#include <Raven.h>

namespace rvn {

	class IconRenderer {
	public:
		IconRenderer();

		void setContext(const ref<Scene>& scene);

		void drawIcons();
	private:
		void drawIconAt(const glm::vec3& position, const ref<SubTexture2D>& icon, std::uint32_t entityID);
	private:
		ref<Texture2D> _atlas;
		ref<SubTexture2D> _camera;
		ref<Scene> _scene;

		SceneCamera* _mainCam = nullptr;
		TransformComponent* _mainCamTransform;
	};

}