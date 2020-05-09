#pragma once
#include <Raven_Core/event/EventSystem.h>
#include <Raven/application/MouseCodes.h>

namespace rvn {
	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y) : Event(EventType::EVENT_MOUSE_MOVED, "MouseMovedEvent"), _x(x), _y(y) { }
		inline float getX() const { return _x; }
		inline float getY() const { return _y; }
	private:
		float _x, _y;
	};
	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOff, float yOff) : Event(EventType::EVENT_MOUSE_SCROLLED, "MouseScrolledEvent"), _xOffset(xOff), _yOffset(yOff) { }
		inline float getXOffset() const { return _xOffset; }
		inline float getYOffset() const { return _yOffset; }
	private:
		float _xOffset, _yOffset;
	};
	class MouseEvent : public Event {
	public:
		inline MouseCode getMouseButton() const { return _button; }
	protected:
		MouseEvent(MouseCode button, EventType type, char* name) : Event(type, name), _button(button) {}
		MouseCode _button;
	};
	class MouseButtonPressedEvent : public MouseEvent {
	public:
		MouseButtonPressedEvent(MouseCode button) : MouseEvent(button, EventType::EVENT_MOUSE_BUTTON_PRESSED, "MousePressedEvent") { }
	};
	class MouseButtonReleasedEvent : public MouseEvent {
	public:
		MouseButtonReleasedEvent(MouseCode button) : MouseEvent(button, EventType::EVENT_MOUSE_BUTTON_RELEASED, "MouseReleasedEvent") { }
	};
}