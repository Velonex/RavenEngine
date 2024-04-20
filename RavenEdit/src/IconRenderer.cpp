#include "IconRenderer.h"
#include <filesystem>
#include <entt.hpp>

namespace rvn {

	static const std::string ICON_ATLAS_PATH = "assets/internal/textures/icons/icon_atlas.png";

	IconRenderer::IconRenderer()
	{
		ASSERT(std::filesystem::exists(ICON_ATLAS_PATH), "Icon atlas is missing!");
		_atlas = Texture2D::create(ICON_ATLAS_PATH);
		_camera = SubTexture2D::createFromCoords(_atlas, { 0.0f, 15.0f }, { 64.0f, 64.0f });
		_primaryCamera = SubTexture2D::createFromCoords(_atlas, { 1.0f, 15.0f }, { 64.0f, 64.0f });
	}

	void IconRenderer::setContext(const ref<Scene>& scene)
	{
		_scene = scene;
	}

	static const glm::vec3 zAxisDir = { 0.0f, 0.0f, 1.0f };

	using rotation = glm::vec3;

	rotation calcRot(const glm::vec3& entity, const glm::vec3& cam) {
		glm::vec3 dxyz = { cam.x - entity.x, cam.y - entity.y, cam.z - entity.z };
		dxyz = glm::normalize(dxyz);
		glm::vec3 dxz = { cam.x - entity.x, 0.0f, cam.z - entity.z };
		dxz = glm::normalize(dxz);
		glm::vec3 zRotVec = { cam.x - entity.x, cam.y - entity.y, 0.0f };
		zRotVec = glm::normalize(zRotVec);
		rotation ret;
		if (dxyz.y != dxz.y) {
			ret.x = glm::acos(glm::dot(dxz, dxyz));
			ret.x *= dxyz.y <= 0 ? 1 : -1;
		}
		else ret.x = 0;
		ret.y = glm::acos(glm::dot(zAxisDir, dxz));
		ret.y *= dxz.x <= 0 ? -1 : 1;
		ret.z = 0;

		return ret;
	}

	void IconRenderer::drawIconAt(const glm::vec3& position, const ref<SubTexture2D>& icon, std::uint32_t entityID)
	{
		auto rot = glm::toMat4(glm::quat(calcRot(position, _editorCamera->getPosition())));

		glm::mat4 iconTransform = glm::translate(glm::mat4(1.0f), position)
			* rot
			* glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Renderer2D::drawQuad(
			iconTransform,
			icon,
			glm::vec4(1.0f),
			1.0f,
			entityID);
	}

	void IconRenderer::drawIcons(EditorCamera& camera)
	{
		_editorCamera = &camera;
		{
			auto view = _scene->_registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view) {
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.primary)
					drawIconAt(transform.translation, _primaryCamera, (std::uint32_t)entity);
				else
					drawIconAt(transform.translation, _camera, (std::uint32_t)entity);
			}
		}
	}

}