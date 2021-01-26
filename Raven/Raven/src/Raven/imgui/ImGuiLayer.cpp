#include <pch.h>
#include "ImGuiLayer.h"

//#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#define IMGUI_IMPL_API
#include <imgui.h>
#include <imgui_internal.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <Raven/application/Application.h>
#include <GLFW/glfw3.h>


namespace rvn {
	ImGuiLayer::ImGuiLayer(Window* window) : Layer("ImGuiLayer")
	{
		_window = window;
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::onAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		setDarkColorTheme();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)_window->getNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiLayer::onDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void ImGuiLayer::onEvent(Event* e)
	{
		if (_blockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e->handled |= (e->getType() == EventType::EVENT_KEY_PRESSED || e->getType() == EventType::EVENT_KEY_RELEASED || e->getType() == EventType::EVENT_KEY_TYPED) & io.WantCaptureKeyboard;
			e->handled |= (e->getType() == EventType::EVENT_MOUSE_BUTTON_PRESSED || e->getType() == EventType::EVENT_MOUSE_BUTTON_RELEASED || e->getType() == EventType::EVENT_MOUSE_MOVED || e->getType() == EventType::EVENT_MOUSE_SCROLLED)  & io.WantCaptureKeyboard;
			LOG_ENGINE_TRACE("{}", e->handled);
		}
	}
	void ImGuiLayer::onImGuiRender()
	{
	}
	void ImGuiLayer::beginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void ImGuiLayer::endFrame()
	{
		ImGuiIO& io = ImGui::GetIO();
#pragma warning(push)
#pragma warning(disable : 4244)
		io.DisplaySize = ImVec2(_window->getWidth(), _window->getHeight());
#pragma warning(pop)
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* window = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(window);
		}
	}
	/* To be changed */
	void ImGuiLayer::setDarkColorTheme()
	{
		auto& colors = ImGui::GetStyle().Colors;
		ImGui::GetStyle().Alpha = 1.0f;
		ImGui::GetStyle().FrameRounding = 0.0f;
		ImGui::GetStyle().TabRounding = 0.0f;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
		colors[ImGuiCol_Text] = ImVec4{ 0.85f, 0.85f, 0.85f, 1.0f };
		colors[ImGuiCol_Border] = ImVec4{ 0.85f, 0.85f, 0.85f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.11f, 0.11f, 0.11f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.173f, 0.173f, 0.173f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.231f, 0.231f, 0.231f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.51f, 0.0f, 0.67f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.46f, 0.0f, 0.604f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.29f, 0.0f, 0.38f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.51f, 0.0f, 0.67f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.46f, 0.0f, 0.604f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.29f, 0.0f, 0.38f, 1.0f };
		colors[ImGuiCol_CheckMark] = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}
}