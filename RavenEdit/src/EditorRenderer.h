#pragma once

#include "EditorCamera.h"
#include "IconRenderer.h"

namespace rvn {

	class EditorRenderer {
	public:
		void setContext(const ref<Scene>& context);

		void updateCamera(Timestep ts);
		void onViewportResize(std::uint32_t width, std::uint32_t height);

		void onUpdate(Timestep ts);
		void onEvent(Event* e);

		EditorCamera& getCamera();
	private:
		ref<Scene> _context;
		EditorCamera _camera;
		std::uint32_t _viewportWidth = 0, _viewportHeight = 0;
		IconRenderer _iconRenderer;
	};

}