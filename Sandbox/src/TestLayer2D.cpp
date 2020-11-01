#include "TestLayer2D.h"
#include <string>
#include <Raven/rendering/Renderer.h>
#include <imgui.h>

void TestLayer2D::onAttach()
{
	camera = rvn::OrthographicCameraController(16.f / 9.f, true);
	chess = rvn::Texture2D::create("assets/textures/chess.png");
	check = rvn::Texture2D::create("assets/textures/check.png");
	atlas = rvn::Texture2D::create("assets/textures/atlas.png");
	fence = rvn::SubTexture2D::createFromCoords(atlas, { 6.0f, 0.0f }, { 128.0f, 128.0f });
}

void TestLayer2D::onUpdate(rvn::Timestep timestep)
{
	camera.onUpdate(timestep);
	rvn::Renderer2D::beginScene(camera.getCamera());
	rvn::Renderer2D::drawQuad({ -1.0f, 0.0f, -0.1f }, { 0.8f, 0.8f }, chess, { 1.0f, 0.0f, 0.0f, 1.0f }, 1.0f);
	rvn::Renderer2D::drawQuad({ 1.0f, 0.0f, -0.05f }, { 0.8f, 0.8f }, check, { 0.0f, 0.8f, 0.0f, 1.0f }, 1.0f);
	rvn::Renderer2D::drawQuad({ 0.0f, 0.0f, 0.0f }, { 0.8f, 0.8f }, fence, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f);
	rvn::Renderer2D::endScene();
}

void TestLayer2D::onDetach()
{
}

void TestLayer2D::onImGuiRender()
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
    ImGui::Begin("DockSpace Demo", &dockspaceOpened, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Docking"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
            if (ImGui::MenuItem("Exit")) { rvn::Application::get().close(); }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void TestLayer2D::onEvent(rvn::Event * e)
{
	camera.onEvent(e);
}
