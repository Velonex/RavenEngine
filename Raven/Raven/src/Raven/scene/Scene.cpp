#include <pch.h>
#include "Scene.h"
#include "Entity.h"
#include <Raven/rendering/Renderer2D.h>
#include "Components.h"
#include <Raven/assets/AssetManager.h>

namespace rvn {
	Scene::Scene(const std::string& name)
		: _name(name)
	{
	}
	Scene::~Scene()
	{
		_registry.each([this](auto entity) {
			this->destroyEntityImpl(entity);
		});
	}
	Entity Scene::createEntity(const std::string& name)
	{
		Entity entity = { _registry.create(), this };
		std::string tag = name.empty() ? "Entity" : name;
		entity.addComponent<TagComponent>(tag);
		entity.addComponent<TransformComponent>();
		entity.addComponent<RUIDComponent>(RUID::generateRUID());
		return entity;
	}
	void Scene::destroyEntity(Entity entity)
	{
		destroyEntityImpl(entity);
	}
	void Scene::destroyEntityImpl(entt::entity handle)
	{
		if (_registry.has<NativeScriptComponent>(handle)) {
			auto& script = _registry.get<NativeScriptComponent>(handle);
			script.destroyScript(&script);
		}
		_registry.destroy(handle);
	}
	void Scene::onUpdate(Timestep ts)
	{
		// Update scripts
		{
			_registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc) {
				if (!nsc.instance) {
					nsc.instance = nsc.instantiateScript();
					nsc.instance->_entity = Entity(entity, this);
					nsc.instance->onCreate();
				}
				nsc.instance->onUpdate(ts);
			});
		}

		Camera* mainCam = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = _registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view) {
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.primary) {
					mainCam = &camera.camera;
					cameraTransform = transform.getTransform();
					break;
				}
			}
		}
		if (mainCam) {
			// Render Quads
			Renderer2D::beginScene(*mainCam, cameraTransform);
			auto view = _registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : view) {
				TransformComponent transform = view.get<TransformComponent>(entity);
				SpriteRendererComponent& spritecomp = view.get<SpriteRendererComponent>(entity);

				if (spritecomp.id != 0) {
					if (!spritecomp.texture || spritecomp.updateTexture) {
						spritecomp.texture = AssetManager::loadTexture(spritecomp.id);
						if (!spritecomp.texture) {
							ASSERT(false, "Invalid ID");
						}
					}
					Renderer2D::drawQuad(transform.getTransform(), spritecomp.texture, spritecomp.color, spritecomp.tilingFactor);
				}
				else {
					Renderer2D::drawQuad(transform.getTransform(), spritecomp.color);
				}
			}
			Renderer2D::endScene();
		}
	}
	void Scene::onViewportResize(std::uint32_t width, std::uint32_t height)
	{
		_viewportWidth = width;
		_viewportHeight = height;
	
		auto view = _registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.fixedAspectRatio)
				cameraComponent.camera.setViewportSize(width, height);
		}
	}
	template<typename T>
	void Scene::onComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}
	template<>
	void Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent& component) {

	}
	template<>
	void Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {

	}
	template<>
	void Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {

	}
	template<>
	void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		component.camera.setViewportSize(_viewportWidth, _viewportHeight);
	}
	template<>
	void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {

	}
	template<>
	void Scene::onComponentAdded<RUIDComponent>(Entity entity, RUIDComponent& component) {

	}
}