#pragma once
#include <Raven_Core/event/EventSystem.h>

namespace rvn {

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() : Event(EventType::EVENT_WINDOW_CLOSE, "WindowCloseEvent") {}
	};

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(int width, int height) : Event(EventType::EVENT_WINDOW_RESIZE, "WindowResizeEvent"), _width(width), _height(height) {}
		int getWidth() const { return _width; }
		int getHeight() const { return _height; }
		void setWidth(int width) { _width = width; }
		void setHeight(int height) { _height = height; }
	private:
		int _width, _height;
	};
}