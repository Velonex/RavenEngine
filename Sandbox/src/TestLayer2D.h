#pragma once
#include <Raven.h>
#include <cstdint>
#include <Raven/rendering/OrthographicCameraController.h>

class TestLayer2D : public rvn::Layer {
public:
	virtual ~TestLayer2D() {}
	virtual void onAttach() override;
	virtual void onUpdate(rvn::Timestep timestep) override;
	virtual void onDetach() override;
	virtual void onImGuiRender() override;
	virtual void onEvent(rvn::Event* e) override;
private:
	rvn::OrthographicCameraController camera = rvn::OrthographicCameraController(16.f / 9.f);
	rvn::ref<rvn::Texture2D> texture;
};