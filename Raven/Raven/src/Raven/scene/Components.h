#pragma once

#include <string>

#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/matrix_transform.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"

#include <Raven/rendering/Texture.h>

namespace rvn {

	struct RUIDComponent {
		RUID ruid;

		RUIDComponent() = default;
		RUIDComponent(const RUIDComponent&) = default;
		RUIDComponent(RUID ruid)
			: ruid(ruid) {}
	};
	struct TagComponent {
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(std::string tag)
			: tag(tag) {}
	};
	struct TransformComponent {
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
		
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(glm::vec3 translation)
			: translation(translation) {}

		glm::mat4 getTransform() const {
			glm::mat4 rot = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4(1.0f), translation)
				* rot
				* glm::scale(glm::mat4(1.0f), scale);
		}
	};
	struct SpriteRendererComponent {
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		std::uint64_t id = 0;
		float tilingFactor = 1.0f;
		ref<Texture2D> texture;
		bool updateTexture = false;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(glm::vec4 color)
			: color(color) {}
		SpriteRendererComponent(glm::vec4 color, std::uint64_t id, float tilingFactor)
			: color(color), id(id), tilingFactor(tilingFactor) {}
	};
	struct CameraComponent {
		SceneCamera camera;
		bool primary = true;
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity* (*instantiateScript)();
		void(*destroyScript)(NativeScriptComponent*);

		template<typename T>
		void bind() {
			instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			destroyScript = [](NativeScriptComponent* nsc) { nsc->instance->onDestroy(); delete nsc->instance; nsc->instance = nullptr; };
		}
	};
}