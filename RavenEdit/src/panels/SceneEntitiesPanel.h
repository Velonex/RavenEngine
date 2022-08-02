#pragma once

#include <Raven/scene/Scene.h>
#include <Raven/scene/Entity.h>
#include <Raven_Core/datatypes/Memory.h>

namespace rvn {

	struct SpriteRendererComponent;

	class SceneEntitiesPanel {
	public:
		SceneEntitiesPanel() = default;
		SceneEntitiesPanel(const ref<Scene>& scene);

		void setContext(const ref<Scene>& scene);
		void setSelectedEntity(Entity entity);

		void onImGuiRender();
	private:
		void drawEntityNode(Entity entity);
		void drawPropertiesPanel(Entity entity);
	private:
		ref<Scene> _context;
		Entity _selectionContext;
	};
}