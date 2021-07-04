#pragma once

#include <Raven/scene/Scene.h>
#include <Raven/scene/Entity.h>
#include <Raven_Core/datatypes/Memory.h>

namespace rvn {

	class SceneSettingsPanel {
	public:
		SceneSettingsPanel() = default;
		SceneSettingsPanel(const ref<Scene>& scene);

		void setContext(const ref<Scene>& scene);

		void onImGuiRender();
	private:
		void drawSceneSettings();
	private:
		ref<Scene> _context;
	};

}