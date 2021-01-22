#include "EditorLayer.h"
#include <imgui.h>

namespace rvn {

    EditorLayer::EditorLayer()
        : Layer("Editor Layer") , _cameraController(1280.0f / 720.0f, true)
    {
    }

    void EditorLayer::onAttach()
    {
        _chess = Texture2D::create("assets/textures/chess.png");

        FramebufferSpecification fbSpec;
        fbSpec.width = 1280;
        fbSpec.height = 720;
        _framebuffer = Framebuffer::create(fbSpec);

        _activeScene = createRef<Scene>();

        _testEntity = _activeScene->createEntity("Test");
        _testEntity.addComponent<SpriteRendererComponent>(glm::vec4( 1.0f, 1.f, 0.f, 1.0f ));
        if (_testEntity.hasComponent<TransformComponent>()) {
            _testEntity.getComponent<TransformComponent>().scale.x = 2.0f;
        }

        _cameraEntity = _activeScene->createEntity("Camera");
        auto& camController = _cameraEntity.addComponent<CameraComponent>();
        class NotCameraController : public ScriptableEntity
        {
        public:
            virtual void onCreate() override
            {
                auto& translation = getComponent<TransformComponent>().translation;
                translation.x = rand() % 10 - 5.0f;
            }

            virtual void onDestroy() override
            {
            }

            virtual void onUpdate(Timestep ts) override
            {
                auto& translation = getComponent<TransformComponent>().translation;

                float speed = 5.0f;

                if (Input::isKeyPressed(KEY_A))
                    translation.x -= speed * ts;
                if (Input::isKeyPressed(KEY_D))
                    translation.x += speed * ts;
                if (Input::isKeyPressed(KEY_W))
                    translation.y += speed * ts;
                if (Input::isKeyPressed(KEY_S))
                    translation.y -= speed * ts;
            }
        };
        _testEntity.addComponent<NativeScriptComponent>().bind<NotCameraController>();
    }

    void EditorLayer::onDetach()
    {
    }

    void EditorLayer::onUpdate(Timestep ts)
    {
        if (auto spec = _framebuffer->getSpecification(); _viewportSize.x > 0.0f && _viewportSize.y > 0.0f &&
            (spec.width != _viewportSize.x || spec.height != _viewportSize.y)) {
            _framebuffer->resize((std::uint32_t)_viewportSize.x, (std::uint32_t)_viewportSize.y);
            _cameraController.onResize(_viewportSize.x, _viewportSize.y);

            _activeScene->onViewportResize((std::uint32_t)_viewportSize.x, (std::uint32_t)_viewportSize.y);
        }

        //_cameraController.onUpdate(ts);
      
        _framebuffer->bind();

        RenderCommand::setClearColor({ 0.8f, 0.8f, 0.8f, 0.8f });
        RenderCommand::clear();

        //rvn::Renderer2D::beginScene(_cameraController.getCamera());
        //rvn::Renderer2D::drawQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f }, { 0.2f, 0.8f, 0.3f, 1.0f });
        //rvn::Renderer2D::drawQuad({ -1.0f, 0.0f, -0.1f }, { 0.8f, 0.8f }, _chess, { 1.0f, 0.0f, 0.0f, 1.0f }, 1.0f);
        //rvn::Renderer2D::endScene();

        _activeScene->onUpdate(ts);

        _framebuffer->unbind();
    }

    void EditorLayer::onEvent(Event* e)
    {
        //_cameraController.onEvent(e);
    }

    void EditorLayer::onImGuiRender()
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

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
                if (ImGui::MenuItem("Exit")) { Application::get().close(); }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        
        ImGui::Begin("Viewport");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        std::uint32_t texID = _framebuffer->getColorAttachmentID();
        ImGui::Image(reinterpret_cast<void*>(texID), ImVec2{ _viewportSize.x, _viewportSize.y }, { 0, 1 }, { 1,0 });
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

}