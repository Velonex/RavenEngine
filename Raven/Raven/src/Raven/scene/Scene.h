#pragma once
#include <entt.hpp>
#include <Raven/application/Timestep.h>
#include <Raven/rendering/OrthographicCamera.h>
#include <string>

namespace rvn {

	class Entity;

	class Scene {
		friend class Entity;
		friend class SceneEntitiesPanel;
		friend class SceneSerializer;
		friend class EditorLayer;
	public:
		Scene(const std::string& name = "Untitled scene");
		~Scene();

		Entity createEntity(const std::string& name = std::string());
		void destroyEntity(Entity entity);

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
	};
}