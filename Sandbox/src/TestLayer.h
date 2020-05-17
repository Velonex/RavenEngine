#pragma once
#include <Raven.h>
#include <glad/glad.h>
#include <cstdint>

class TestLayer : public rvn::Layer {
public:
	virtual void onAttach() override;
	virtual void onUpdate(rvn::Timestep timestep) override;
	virtual void onDetach() override;
	virtual void onImGuiRender() override;
	virtual void onEvent(rvn::Event* e) override;
private:
	std::uint32_t vao = 0, vbo = 0, ibo = 0, shader = 0;
};