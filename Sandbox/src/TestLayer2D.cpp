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
	rvn::RenderCommand::setClearColor({ 1.0f, 0.0f, 1.0f, 1.0f });
	rvn::RenderCommand::clear();
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
}

void TestLayer2D::onEvent(rvn::Event * e)
{
	camera.onEvent(e);
}
