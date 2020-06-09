#pragma once
#include <Raven.h>
#include <glad/glad.h>
#include <cstdint>

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
};