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