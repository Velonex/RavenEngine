#pragma once
#include <entt.hpp>
#include <Raven/application/Timestep.h>
#include <Raven/rendering/OrthographicCamera.h>
#include <string>
#include <Raven/scene/SceneCamera.h>
#include <Raven/rendering/Shader.h>

namespace rvn {

	class Entity;

	class Scene {
		friend class Entity;
		friend class SceneEntitiesPanel;
		friend class SceneSettingsPanel;
		friend class SceneSerializer;
		friend class IconRenderer;
		friend class EditorLayer;
		friend class EditorRenderer;
	public:
		Scene(const std::string& name = "Untitled scene");
		~Scene();

		Entity createEntity(const std::string& name = std::string());
		void destroyEntity(Entity entity);

		Entity getEntityByID(std::uint32_t id);

		const SceneCamera* getMainCamera() const;
		glm::mat4 getMainCameraTransform() const;

		void onUpdate(Timestep ts);
		void onViewportResize(std::uint32_t width, std::uint32_t height);

		const std::string& getName() const { return _name; }
		void setName(const std::string& name) { _name = name; }

		const glm::vec4& getClearColor() const { return _clearColor; }
	private:
		template <typename T>
		void onComponentAdded(Entity entity, T& component);
		void destroyEntityImpl(entt::entity handle);
	private:
		entt::registry _registry;
		std::uint32_t _viewportWidth = 0, _viewportHeight = 0;
	private:
		std::string _name = "";
		glm::vec4 _clearColor = { 0.7f, 0.7f, 0.7f, 1.0f };
	private:
		ref<Shader> _3dDefaultShader = nullptr;

		SceneCamera* _mainCam = nullptr;
		glm::mat4 _mainCamTransform;
	};
}