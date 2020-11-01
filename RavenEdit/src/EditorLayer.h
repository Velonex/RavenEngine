#pragma once
#include <Raven/layers/Layer.h>

class EditorLayer : public rvn::Layer {
public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate(rvn::Timestep ts) override;
	virtual void onEvent(rvn::Event* e) override;
	virtual void onImGuiRender() override;
};