#include "TestLayer2D.h"
#include <string>
#include <Raven/rendering/Renderer.h>
#include <imgui.h>

void TestLayer2D::onAttach()
{
	camera = rvn::OrthographicCameraController(16.f / 9.f, true);
	texture = rvn::Texture2D::create("assets/textures/chess.png");
}

void TestLayer2D::onUpdate(rvn::Timestep timestep)
{
	camera.onUpdate(timestep);
	rvn::Renderer2D::beginScene(camera.getCamera());
	rvn::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 0.8f, 0.8f }, texture, { 1.0f, 0.0f, 0.0f, 1.0f }, 1.0f);
	rvn::Renderer2D::endScene();
}

void TestLayer2D::onDetach()
{
}

void TestLayer2D::onImGuiRender()
{
}

void TestLayer2D::onEvent(rvn::Event * e)
{
	camera.onEvent(e);
}
