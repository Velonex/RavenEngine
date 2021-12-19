#include "SceneEntitiesPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include <Raven/scene/Components.h>
#include <Raven/assets/Filesystem.h>
#include <Raven/utils/PlatformUtils.h>
#include <Raven/assets/IdLookup.h>

#include <cstring>
#include <filesystem>

namespace rvn {

	SceneEntitiesPanel::SceneEntitiesPanel(const ref<Scene>& scene)
	{
		setContext(scene);
	}

	void SceneEntitiesPanel::setContext(const ref<Scene>& scene)
	{
		_context = scene;
		_selectionContext = {};
	}

	void SceneEntitiesPanel::onImGuiRender()
	{
		ImGui::Begin("Scene Entities");
		_context->_registry.each([&](auto entityID) {
			Entity entity = { entityID, _context.get() };
			drawEntityNode(entity);
		});
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			_selectionContext = {};
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create empty entity")) _context->createEntity("Empty Entity");
			ImGui::EndPopup();
		}
		ImGui::End();
		ImGui::Begin("Entity Properties");
		if (_selectionContext) {
			drawPropertiesPanel(_selectionContext);
		}
		else {
			ImGui::Text("No entity selected.");
		}
		ImGui::End();
	}

	void SceneEntitiesPanel::drawEntityNode(Entity entity)
	{
		std::string& name = entity.getComponent<TagComponent>().tag;

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4{ 0.173f, 0.173f, 0.173f, 1.0f });
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (entity == _selectionContext) flags |= ImGuiTreeNodeFlags_Selected;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
		if (ImGui::IsItemClicked()) {
			_selectionContext = entity;
		}
		bool shouldBeDeleted = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete entity")) shouldBeDeleted = true;
			ImGui::EndPopup();
		}
		if (opened)
		{
			ImGuiViewportFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened2 = ImGui::TreeNodeEx((void*)9817239, flags, name.c_str());
			if (opened2)
				ImGui::TreePop();
			ImGui::TreePop();
		}
		ImGui::PopStyleColor(1);
		if (shouldBeDeleted) {
			_context->destroyEntity(entity);
			_selectionContext = {};
		}
	}

	template<typename Component, typename UIFunc>
	static void drawComponent(const std::string& name, Entity entity, UIFunc drawUI) {
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.hasComponent<Component>()) {
			float contentRegionAvail = ImGui::GetContentRegionAvailWidth();
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			bool opened = ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), treeNodeFlags, name.c_str());
			ImGui::SameLine((contentRegionAvail - lineHeight * 0.5f));
			
			if (ImGui::Button((std::string("v##") + std::to_string(typeid(Component).hash_code())).c_str(), { lineHeight, lineHeight })) {
				ImGui::OpenPopup("ComponentSettings");
			}
			bool shouldBeDeleted = false;
			if (ImGui::BeginPopup("ComponentSettings")) {
				if (ImGui::MenuItem("Remove component")) shouldBeDeleted = true;
				ImGui::EndPopup();
			}
			if (opened) {
				drawUI(entity.getComponent<Component>());
				ImGui::TreePop();
			}
			if (shouldBeDeleted) {
				entity.removeComponent<Component>();
			}
		}
	}
	void SceneEntitiesPanel::drawPropertiesPanel(Entity entity)
	{
		// Draw tag
		ASSERT(entity.hasComponent<TagComponent>(), "Entity needs to have a tag component");
		auto& tag = _selectionContext.getComponent<TagComponent>().tag;
		char buf[128];
		strncpy_s(buf, tag.c_str(), tag.length());
		if (ImGui::InputText("##TagInput", buf, sizeof(buf)))
		{
			tag = buf;
		}
		// Draw add components button;
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("AddComponentPopUp");
		}
		
		if (ImGui::BeginPopup("AddComponentPopUp")) {
			if (!entity.hasComponent<TransformComponent>())
				if (ImGui::MenuItem("Transform")) {
					entity.addComponent<TransformComponent>();
				}
			if (!entity.hasComponent<SpriteRendererComponent>())
				if (ImGui::MenuItem("Sprite renderer")) {
					entity.addComponent<SpriteRendererComponent>();
				}
			if (!entity.hasComponent<CameraComponent>())
				if (ImGui::MenuItem("Camera")) {
					entity.addComponent<CameraComponent>();
				}
			ImGui::EndPopup();
		}
		// Draw components
		drawComponent<TransformComponent>("Transform", entity, [&](TransformComponent& transform) {
			ImGui::Text("Translation"); ImGui::SameLine(ImGui::GetContentRegionAvailWidth() / 3);
			ImGui::DragFloat3("##Translation", glm::value_ptr(transform.translation), 0.01f);
			glm::vec3 rotation = glm::degrees(transform.rotation);
			ImGui::Text("Rotation"); ImGui::SameLine(ImGui::GetContentRegionAvailWidth() / 3);
			ImGui::DragFloat3("##Rotation", glm::value_ptr(rotation), 1.0f);
			transform.rotation = glm::radians(rotation);
			ImGui::Text("Scale"); ImGui::SameLine(ImGui::GetContentRegionAvailWidth() / 3);
			ImGui::DragFloat3("##Scale", glm::value_ptr(transform.scale), 0.01f);
		});
		drawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [&](SpriteRendererComponent& component) {
			ImGui::ColorEdit4("Color##SRPicker", glm::value_ptr(component.color));
			if (ImGui::Button("Set Texture")) {
				std::optional<std::string> path = FileDialogs::openFile("Image File (PNG, JPG, BMP, TGA, GIF)\0*.png;*.jpg;*.jpe;*.jif;*.jfif;*.tga;*.tpic;*.bmp;*.dip;*.gif");
				if (path) {
					component.id = IdLookup::getID(path.value());
					component.updateTexture = true;
				}
			}
			ImGui::SameLine();
			ImGui::Text(component.id == 0 ? "Default texture" : IdLookup::getPath(component.id).c_str());
			if (ImGui::Button("Reset texture")) {
				component.id = 0;
				component.texture.reset();
			}
			ImGui::Text("Tiling factor"); ImGui::SameLine();
			ImGui::DragFloat("##Tiling Factor", &component.tilingFactor, 0.01f, 0.01f, 10.0f);
		});
		drawComponent<CameraComponent>("Camera", entity, [&](CameraComponent& component) {
			ImGui::Checkbox("Primary camera", &component.primary);

			auto& cam = component.camera;

			const char* projectionTypes[] = { "Perspective", "Orthographic" };
			const char* currentProjectionType = projectionTypes[(int)cam.getProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionType)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentProjectionType = projectionTypes[i];
					if (ImGui::Selectable(projectionTypes[i], isSelected)) {
						currentProjectionType = projectionTypes[i];
						cam.setProjectionType((SceneCamera::ProjectionType)i);
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (cam.getProjectionType() == SceneCamera::ProjectionType::Orthographic) {
				ImGui::Text("Size"); ImGui::SameLine();
				float orthoSize = cam.getOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					cam.setOrthographicSize(orthoSize);
				ImGui::Text("Near"); ImGui::SameLine();
				float orthoNear = cam.getOrthographicNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					cam.setOrthographicNearClip(orthoNear);
				ImGui::Text("Far"); ImGui::SameLine();
				float orthoFar = cam.getOrthographicFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					cam.setOrthographicFarClip(orthoFar);
				
				ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspectRatio);
			}
			if (cam.getProjectionType() == SceneCamera::ProjectionType::Perspective) {
				ImGui::Text("Vertical FOV"); ImGui::SameLine();
				float perspectiveVertFOV = glm::degrees(cam.getPerspectiveFOV());
				if (ImGui::DragFloat("Vertical FOV", &perspectiveVertFOV))
					cam.setPerspectiveFOV(glm::radians(perspectiveVertFOV));
				ImGui::Text("Near"); ImGui::SameLine();
				float perspectiveNear = cam.getPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					cam.setPerspectiveNearClip(perspectiveNear);
				ImGui::Text("Far"); ImGui::SameLine();
				float perspectiveFar = cam.getPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					cam.setPerspectiveFarClip(perspectiveFar);
			}
		});
	}
}