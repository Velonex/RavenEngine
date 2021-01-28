#pragma once
#pragma warning(push, 0)
#include <entt.hpp>
#pragma warning(pop)
#include "Scene.h"
#include <Raven_Core/core/Assert.h>
#include <Raven_Core/datatypes/RavenUniqueID.h>

namespace rvn {

	class Entity {
		friend class SceneSerializer;
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		// Components
		template<typename T, typename... Args>
		T& addComponent(Args&&... args) {
			ASSERT(!hasComponent<T>(), "Entity already has component!");
			T& comp = _scene->_registry.emplace<T>(_handle, std::forward<Args>(args)...);
			_scene->onComponentAdded<T>(*this, comp);
			return comp;
		}
		template<typename T>
		bool hasComponent() {
			return _scene->_registry.has<T>(_handle);
		}
		template<typename T>
		T& getComponent() {
			ASSERT(hasComponent<T>(), "Entity doesn't have component!");
			return _scene->_registry.get<T>(_handle);
		}
		template<typename T>
		void removeComponent() {
			ASSERT(hasComponent<T>(), "Entity doesn't have component!");
			return _scene->_registry.remove<T>(_handle);
		}

		// Operators
		operator bool() const { return _handle != entt::null; }
		operator entt::entity() const { return _handle; }
		operator std::uint32_t() const { return (std::uint32_t)_handle; }

		bool operator==(const Entity& other) const {
			return other._handle == _handle && other._scene == _scene;
		}
		bool operator!=(const Entity& other) const {
			return !(other == *this);
		}
	protected:
		entt::entity _handle = entt::null;
		Scene* _scene = nullptr;
	};
}