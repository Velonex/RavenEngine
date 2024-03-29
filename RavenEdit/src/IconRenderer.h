#pragma once
#include <Raven.h>
#include "EditorCamera.h"

namespace rvn {

	class IconRenderer {
	public:
		IconRenderer();

		void setContext(const ref<Scene>& scene);

		void drawIcons(EditorCamera& camera);
	private:
		void drawIconAt(const glm::vec3& position, const ref<SubTexture2D>& icon, std::uint32_t entityID);
	private:
		ref<Texture2D> _atlas;

		ref<SubTexture2D> _camera;
		ref<SubTexture2D> _primaryCamera;

		ref<Scene> _scene;

		EditorCamera* _editorCamera = nullptr;
	};

}