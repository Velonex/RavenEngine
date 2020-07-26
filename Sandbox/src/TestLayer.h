#pragma once
#include <Raven.h>
#include <cstdint>
#include <Raven/rendering/OrthographicCameraController.h>

class TestLayer : public rvn::Layer {
public:
	virtual ~TestLayer() {}
	virtual void onAttach() override;
	virtual void onUpdate(rvn::Timestep timestep) override;
	virtual void onDetach() override;
	virtual void onImGuiRender() override;
	virtual void onEvent(rvn::Event* e) override;
private:
	rvn::ref<rvn::VertexArray> vao;
	rvn::ref<rvn::VertexBuffer> vbo;
	rvn::ref<rvn::IndexBuffer> ibo;
	rvn::ref<rvn::Shader> shader;
	rvn::OrthographicCameraController camera = rvn::OrthographicCameraController(16.f / 9.f);
	rvn::ref<rvn::Texture2D> texture;
};