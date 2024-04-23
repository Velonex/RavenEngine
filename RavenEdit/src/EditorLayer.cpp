#include "EditorLayer.h"
#include <imgui.h>
#include <glm.hpp>
#include <Raven/utils/PlatformUtils.h>
#include <filesystem>

namespace rvn {

    EditorLayer::EditorLayer()
        : Layer("Editor Layer"), _frametimes(100)
    {
        for (int i = 0; i < _frametimes.capacity(); i++)
            _frametimes.push_back(0.0f);
    }
    void EditorLayer::onAttach()
    {
        _editorRenderer.init();
        FramebufferSpecification fbSpec;
        fbSpec.width = 1280;
        fbSpec.height = 720;
		AttachmentSpecification defaultColor;
		defaultColor.layout = AttachmentLayout::RGBA8;
		fbSpec.attachments.push_back(defaultColor);
		AttachmentSpecification id;
		id.layout = AttachmentLayout::RED_UINT;
		fbSpec.attachments.push_back(id);
        AttachmentSpecification depth;
        depth.layout = AttachmentLayout::DEPTH24STENCIL8;
        fbSpec.attachments.push_back(depth);
		_framebuffer = Framebuffer::create(fbSpec);

        _activeScene = createRef<Scene>();

        updateContexts(_activeScene);
    }

    void EditorLayer::onDetach()
    {
    }

    void EditorLayer::onUpdate(Timestep ts)
    {
        if (auto spec = _framebuffer->getSpecification(); _viewportSize.x > 0.0f && _viewportSize.y > 0.0f &&
            (spec.width != _viewportSize.x || spec.height != _viewportSize.y)) {
            _framebuffer->resize((std::uint32_t)_viewportSize.x, (std::uint32_t)_viewportSize.y);
        
            onViewportResize((std::uint32_t)_viewportSize.x, (std::uint32_t)_viewportSize.y);
        }

        _framebuffer->bind();
        
        RenderCommand::setClearColor(_activeScene->getClearColor());
        RenderCommand::clear();

		// Clear id attachment of framebuffer after it is cleared with the clear color to avoid the clear color being an entity id
		_framebuffer->clearAttachment(1, -1);

        if (_viewportFocused && !_isRunning) _editorRenderer.updateCamera(ts);

        if (_isRunning) {
            _activeScene->onUpdate(ts);
        }
        else {
            _editorRenderer.onUpdate(ts);
        }

		if (_viewportHovered)
			_hoveredID = _framebuffer->pixelAt(1, _mousePosInViewport.x, _mousePosInViewport.y);
		else
			_hoveredID = -1;
        
		// A click on the viewport is blocked by ImGui because the viewport window is focused, therefore you would have to click on an entity twice to select it
		// Here's a workaround that lets you select an entity on the first click
		if (Input::isMousePressed(Mouse::Button0)) {
			if (!_mouseDownLast) onMouseLeft();
			_mouseDownLast = true;
		}
		else _mouseDownLast = false;

        _framebuffer->unbind();
        
        _frametimes.push_back(ts.getSeconds());
        _FPSAcc += ts;
        _frameCount++;
    }

    void EditorLayer::onEvent(Event* e)
    {
        //_cameraController.onEvent(e);
        if (_viewportHovered) _editorRenderer.onEvent(e);
        switch (e->getType()) {
        case EventType::EVENT_KEY_PRESSED: {
            onKeyPressed((KeyPressedEvent*)e);
            break;
        }
        }
    }

    void EditorLayer::onImGuiRender()
    {
        beginDockspace();

        if (ImGui::BeginMenuBar())
        {
            drawMenuBar();
        
            ImGui::EndMenuBar();
        }
        
        _sceneEntitiesPanel.onImGuiRender();
        _sceneSettingsPanel.onImGuiRender();

        drawViewport();

        // Debug Info
        ImGui::Begin("Debug Info");
        if (_FPSTimer.hasReached(250)) {
            _lastFPS = 1 / (_FPSAcc / _frameCount);
            _FPSAcc = 0.0f;
            _frameCount = 0;
        }
        ImGui::Text("FPS: %.2f", _lastFPS);
        ImGui::PlotLines("Frametimes", [](void* data, int idx) -> float { return ((CircularBuffer<float>*)data)->operator[](idx); }, (void*)&_frametimes,
            100, 0, 0, 0.0f);
		ImGui::Text("Mouse position in viewport: %i, %i", _mousePosInViewport.x, _mousePosInViewport.y);
		std::string hoveredEntityName = "None";
		if (_hoveredID != -1) 
			hoveredEntityName = _activeScene->getEntityByID(_hoveredID).getComponent<TagComponent>().tag;
		ImGui::Text("Hovered entity: %s", hoveredEntityName.c_str());
        ImGui::Checkbox("Is running", &_isRunning);
		ImGui::End();

        endDockspace();
    }

    void EditorLayer::onKeyPressed(KeyPressedEvent* e)
    {
        if (e->getRepeatCount() > 0) return;

        bool ctrl = Input::isKeyPressed(Key::LeftControl) || Input::isKeyPressed(Key::RightControl);
        bool shift = Input::isKeyPressed(Key::LeftShift) || Input::isKeyPressed(Key::RightShift);

        switch (e->getKeyCode()) {
        case Key::N: {
            if (ctrl) newScene();
            break;
        }
        case Key::S: {
            if (ctrl && shift) saveSceneAs();
            break;
        }
        case Key::O: {
            if (ctrl) openScene();
            break;
        }
        }
    }

	void EditorLayer::onMouseLeft()
	{
		// Select entity if hovered
		if (_hoveredID != -1) {
			_sceneEntitiesPanel.setSelectedEntity(_activeScene->getEntityByID(_hoveredID));
        }
        else {
            if(_viewportHovered)
                _sceneEntitiesPanel.setSelectedEntity({});
        }
	}

    void EditorLayer::onViewportResize(std::uint32_t width, std::uint32_t height)
    {
        _activeScene->onViewportResize(width, height);
        _editorRenderer.onViewportResize(width, height);
    }

    void EditorLayer::updateContexts(const ref<Scene>& scene)
    {
        _sceneEntitiesPanel.setContext(_activeScene);
        _sceneSettingsPanel.setContext(_activeScene);
        _editorRenderer.setContext(_activeScene);
    }

    void EditorLayer::beginDockspace()
    {
        bool dockspaceOpened = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &dockspaceOpened, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

    }

    void EditorLayer::endDockspace()
    {
        ImGui::End();
    }

    void EditorLayer::drawMenuBar()
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) { newScene(); }
            if (ImGui::MenuItem("Open Scene...", "Ctrl+O")) { openScene(); }
            if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) { saveSceneAs(); }
            if (ImGui::MenuItem("Exit")) { Application::get().close(); }
            ImGui::EndMenu();
        }
    }

    void EditorLayer::drawViewport()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

        ImGui::Begin("Viewport");

        _viewportFocused = ImGui::IsWindowFocused();
        _viewportHovered = ImGui::IsWindowHovered();
        Application::get().getImGuiLayer()->blockEvents(!_viewportFocused || !_viewportHovered);

        if (_viewportHovered) {
            auto [mouseX, mouseY] = ImGui::GetMousePos();
            auto [viewportX, viewportY] = ImGui::GetCursorScreenPos();
            int posX = (int)mouseX - (int)viewportX;
            int posY = (int)mouseY - (int)viewportY;
            _mousePosInViewport.x = posX;
            _mousePosInViewport.y = posY;
        }

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        std::uint64_t texID = _framebuffer->getColorAttachmentID();
        ImGui::Image(reinterpret_cast<void*>(texID), ImVec2{ _viewportSize.x, _viewportSize.y }, { 0, 1 }, { 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();
    }
    void EditorLayer::newScene()
    {
        _activeScene = createRef<Scene>();
        onViewportResize((std::uint32_t)_viewportSize.x, (std::uint32_t)_viewportSize.y);

        updateContexts(_activeScene);
    }

    void EditorLayer::openScene()
    {
        std::optional<std::string> filepath = FileDialogs::openFile("All Raven Scene Files\0*.rsc;*.rrsc\0Raven Scene (*.rsc)\0*.rsc\0Raven Runtime Scene (*.rrsc)\0*.rrsc");
        if (filepath) {
            newScene();
            if (std::filesystem::path(filepath.value()).extension() == ".rrsc") SceneSerializer::deserializeRuntime(_activeScene, filepath.value());
            else SceneSerializer::deserialize(_activeScene, filepath.value());
        }
    }

    void EditorLayer::saveSceneAs()
    {
        std::optional<std::string> filepath = FileDialogs::saveFile("Raven Scene (*.rsc)\0*.rsc\0Raven Runtime Scene (*.rrsc)\0*.rrsc");
        if (filepath) {
            if (std::filesystem::path(filepath.value()).extension() == ".rrsc") SceneSerializer::serializeRuntime(_activeScene, filepath.value());
            else SceneSerializer::serialize(_activeScene, filepath.value());
        }
    }

}