#pragma once
#include <entt.hpp>
#include <Raven/application/Timestep.h>
#include <Raven/rendering/OrthographicCamera.h>

namespace rvn {

	class Entity;

	class Scene {
		friend class Entity;
	public:
		Scene();
		~Scene();

		Entity createEntity(const std::string& name = std::string());
		void destroyEntity(Entity entity);

		void onUpdate(Timestep ts);
		void onViewportResize(std::uint32_t width, std::uint32_t height);
	private:
		template <typename T>
		void onComponentAdded(Entity entity, T& component);
		void destroyEntity(entt::entity handle);
	private:
		entt::registry _registry;
		std::uint32_t _viewportWidth = 0, _viewportHeight = 0;
	};
}