#include "SceneEntitiesPanel.h"

#include <imgui.h>

#include <Raven/scene/Components.h>

rvn::SceneEntitiesPanel::SceneEntitiesPanel(const ref<Scene>& scene)
{
	setContext(scene);
}

void rvn::SceneEntitiesPanel::setContext(const ref<Scene>& scene)
{
	_context = scene;
}

void rvn::SceneEntitiesPanel::onImGuiRender()
{
	ImGui::Begin("Scene Entities");
	_context->_registry.each([&](auto entityID) {
		Entity entity = { entityID, _context.get() };
		drawEntityNode(entity);
	});
	ImGui::End();
}

void rvn::SceneEntitiesPanel::drawEntityNode(Entity entity)
{
	std::string& name = entity.getComponent<TagComponent>().tag;

	ImGuiTreeNodeFlags flags = ((_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
	if (ImGui::IsItemClicked()) {
		_selectionContext = entity;
	}
	if (opened)
	{
		ImGuiViewportFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened2 = ImGui::TreeNodeEx((void*)9817239, flags, name.c_str());
		if (opened2)
			ImGui::TreePop();
		ImGui::TreePop();
	}
}
