#pragma once

// This header includes all headers from Raven and its dependencies that are needed to create a project with Raven

// Raven Core
#include <Raven_Core.h>

// Application
#include <Raven/application/Application.h>
#include <Raven/application/Input.h>
#include <Raven/application/KeyCodes.h>
#include <Raven/application/MouseCodes.h>
#include <Raven/application/Timestep.h>
#include <Raven/application/Window.h>

// Events
#include <Raven/events/KeyboardEvents.h>
#include <Raven/events/MouseEvents.h>
#include <Raven/events/WindowEvents.h>

// ImGui
#include <Raven/imgui/ImGuiLayer.h>

// Layers
#include <Raven/layers/Layer.h>
#include <Raven/layers/LayerStack.h>

// Rendering
#include <Raven/rendering/GraphicsContext.h>
#include <Raven/rendering/RenderCommand.h>
#include <Raven/rendering/RendererAPI.h>
#include <Raven/rendering/VertexArray.h>
#include <Raven/rendering/Buffer.h>
#include <Raven/rendering/Shader.h>
#include <Raven/rendering/Texture.h>
#include <Raven/rendering/SubTexture2D.h>
#include <Raven/rendering/OrthographicCamera.h>
#include <Raven/rendering/OrthographicCameraController.h>
#include <Raven/rendering/Renderer.h>
#include <Raven/rendering/Renderer2D.h>
#include <Raven/rendering/Framebuffer.h>
#include <Raven/rendering/Camera.h>

// Scene
#include <Raven/scene/Scene.h>
#include <Raven/scene/Components.h>
#include <Raven/scene/Entity.h>
#include <Raven/scene/ScriptableEntity.h>
#include <Raven/scene/SceneCamera.h>