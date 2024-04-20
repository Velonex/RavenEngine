#include "EditorRenderer.h"

namespace rvn {



	void EditorRenderer::setContext(const ref<Scene>& context)
	{
		_context = context;
		_iconRenderer.setContext(context);
	}

	void EditorRenderer::updateCamera(Timestep ts)
	{
		_camera.onUpdate(ts);
	}

	void EditorRenderer::onViewportResize(std::uint32_t width, std::uint32_t height)
	{
		_viewportWidth = width;
		_viewportHeight = height;

		_camera.setViewportSize(width, height);
	}

	void EditorRenderer::onUpdate(Timestep ts)
	{
		// Render Quads
		Renderer2D::beginScene(_camera, _camera.getTransform());
		auto view = _context->_registry.view<TransformComponent, SpriteRendererComponent>();
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
				Renderer2D::drawQuad(transform.getTransform(), spritecomp.texture, spritecomp.color, spritecomp.tilingFactor, (std::uint32_t)entity);
			}
			else {
				Renderer2D::drawQuad(transform.getTransform(), spritecomp.color, (std::uint32_t)entity);
			}
		}
		// Draw icons
		_iconRenderer.drawIcons(_camera);

		Renderer2D::endScene();
	}

	void EditorRenderer::onEvent(Event* e)
	{
		_camera.onEvent(e);
	}

	EditorCamera& EditorRenderer::getCamera()
	{
		return _camera;
	}

}