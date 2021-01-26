#pragma once
#include <Raven/application/Application.h>
#include <utility>
#include <Raven/application/MouseCodes.h>
#include <Raven/application/KeyCodes.h>
#include <Raven_Core/core/Platform.h>
#include <glm.hpp>

namespace rvn {
	class Input {
		friend class Application;
	public:
		static bool isKeyPressed(KeyCode key);
		static bool isMousePressed(MouseCode button);
		static float getMouseX();
		static float getMouseY();
		static glm::vec2 getMousePos();
	};
}