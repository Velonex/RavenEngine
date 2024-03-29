#pragma once
#include <Raven/layers/Layer.h>
#include <Raven/application/Window.h>

struct Application;

namespace rvn {

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer(Window* window);
		~ImGuiLayer();
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onEvent(Event* e) override;
		virtual void onImGuiRender() override;

		void beginFrame();
		void endFrame();

		void setDarkColorTheme();

		void blockEvents(bool block) { _blockEvents = block; }
	private:
		Window* _window;
		bool _blockEvents = false;
	};

}