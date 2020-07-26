#include "TestLayer2D.h"
#include <string>
#include <Raven/rendering/Renderer.h>
#include <imgui.h>

struct Vertex {
	float position[3];
	float tex_coords[2];
};

void TestLayer2D::onAttach()
{
	camera = rvn::OrthographicCameraController(16.f / 9.f, true);
}

void TestLayer2D::onUpdate(rvn::Timestep timestep)
{
	camera.onUpdate(timestep);
	rvn::Renderer2D::beginScene(camera.getCamera());
	rvn::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	//for (int y = 0; y < 50; y++) {
	//	for (int x = 0; x < 50; x++) {
	//		rvn::Renderer2D::drawQuad({ (float)x, (float)y, 0.0f }, { 0.9f, 0.9f }, { 0.5f, 1.0f, 0.1f, 1.0f });
	//	}
	//}
	rvn::Renderer2D::drawRotatedQuad({ -0.5f, 0.0f, -0.1f }, { 0.2f, 0.4f }, 45.f, { 0.5f, 0.8f, 0.1f, 1.0f });
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
