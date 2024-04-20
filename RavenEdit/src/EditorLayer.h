#pragma once
#include <Raven.h>
#include <panels/SceneEntitiesPanel.h>
#include <panels/SceneSettingsPanel.h>
#include "EditorRenderer.h"

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
		void onMouseLeft();
		void onViewportResize(std::uint32_t width, std::uint32_t height);

		void updateContexts(const ref<Scene>& scene);

		// ImGui
		void beginDockspace();
		void endDockspace();

		void drawMenuBar();

		// Viewport
		void drawViewport();

		void newScene();
		void openScene();
		void saveSceneAs();
	private:
		bool _firstTime = true;

		bool _isRunning = false;

		EditorRenderer _editorRenderer;

		ref<Framebuffer> _framebuffer;

		ref<Scene> _activeScene;

		bool _viewportFocused = false, _viewportHovered = false;

		glm::vec2 _viewportSize = { 0.0f, 0.0f };
		glm::vec<2, int> _mousePosInViewport;
		std::uint32_t _hoveredID = -1;

		SceneEntitiesPanel _sceneEntitiesPanel;
		SceneSettingsPanel _sceneSettingsPanel;

		// FPS
		CircularBuffer<float> _frametimes;
		Timer _FPSTimer;
		float _lastFPS = 0.0f;
		float _FPSAcc = 0.0f;
		std::uint32_t _frameCount = 0;

		bool _mouseDownLast = false;
	};
}