#pragma once
#include <Raven.h>

namespace rvn {
	class EditorLayer : public Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void onUpdate(Timestep ts) override;
		virtual void onEvent(Event* e) override;
		virtual void onImGuiRender() override;
	private:
		OrthographicCameraController _cameraController;

		ref<Texture2D> _chess;
		ref<Framebuffer> _framebuffer;

		ref<Scene> _activeScene;
		Entity _testEntity;
		Entity _cameraEntity;

		glm::vec2 _viewportSize = { 0.0f, 0.0f };
	};
}