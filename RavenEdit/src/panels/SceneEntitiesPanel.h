#pragma once

#include <Raven/scene/Scene.h>
#include <Raven/scene/Entity.h>
#include <Raven_Core/datatypes/Memory.h>

namespace rvn {

	class SceneEntitiesPanel {
	public:
		SceneEntitiesPanel() = default;
		SceneEntitiesPanel(const ref<Scene>& scene);

		void setContext(const ref<Scene>& scene);

		void onImGuiRender();
	private:
		void drawEntityNode(Entity entity);
		void drawPropertiesPanel(Entity entity);
		void drawSceneSettings();
	private:
		ref<Scene> _context;
		Entity _selectionContext;
	};
}