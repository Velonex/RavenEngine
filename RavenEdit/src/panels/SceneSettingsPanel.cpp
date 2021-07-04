#include "SceneSettingsPanel.h"

#include <imgui.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>


namespace rvn {
	SceneSettingsPanel::SceneSettingsPanel(const ref<Scene>& scene)
	{
		setContext(scene);
	}
	void SceneSettingsPanel::setContext(const ref<Scene>& scene)
	{
		_context = scene;
	}
	void SceneSettingsPanel::onImGuiRender()
	{
		ImGui::Begin("Scene Settings");
		drawSceneSettings();
		ImGui::End();
	}
	void SceneSettingsPanel::drawSceneSettings()
	{
		float spacing = ImGui::GetContentRegionAvailWidth() / 3;
		ImGui::Text("Scene Name"); ImGui::SameLine(spacing);
		char buf[128];
		strncpy_s(buf, _context->_name.c_str(), _context->_name.length());
		if (ImGui::InputText("##SceneNameInput", buf, sizeof(buf)))
		{
			_context->_name = buf;
		}
		ImGui::Text("Clear Color"); ImGui::SameLine(spacing);
		ImGui::ColorEdit4("##ClearColorEdit", glm::value_ptr(_context->_clearColor));
	}
}