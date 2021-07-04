#pragma once
#include <Raven.h>
#include <panels/SceneEntitiesPanel.h>
#include <panels/SceneSettingsPanel.h>

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
		void onKeyPressed(KeyPressedEvent* e);

		void newScene();
		void openScene();
		void saveSceneAs();
	private:
		OrthographicCameraController _cameraController;

		bool firstTime = true;

		ref<Texture2D> _chess;
		ref<Framebuffer> _framebuffer;

		ref<Scene> _activeScene;
		Entity _testEntity;
		Entity _cameraEntity;

		bool _viewportFocused = false, _viewportHovered = false;

		glm::vec2 _viewportSize = { 0.0f, 0.0f };

		SceneEntitiesPanel _sceneEntitiesPanel;
		SceneSettingsPanel _sceneSettingsPanel;

		CircularBuffer<float> _frametimes;
		float _frametimesArray[100];
	};
}