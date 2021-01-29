#pragma once
#include <Raven_Core/event/EventSystem.h>
#include <Raven/application/KeyCodes.h>

namespace rvn {
	class KeyEvent : public Event {
	public:
		inline KeyCode getKeyCode() const { return _key; }
	protected:
		KeyEvent(KeyCode key, EventType type, char* name) : Event(type, name), _key(key) {}
		KeyCode _key;
	};
	class KeyPressedEvent : public KeyEvent {
	public:
		inline int getRepeatCount() { return _repeatCount; }
		KeyPressedEvent(KeyCode key, int repeats) : KeyEvent(key, EventType::EVENT_KEY_PRESSED, "KeyPressedEvent"), _repeatCount(repeats) {}
	private:
		int _repeatCount;
	};
	class KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(KeyCode key) : KeyEvent(key, EventType::EVENT_KEY_TYPED, "KeyTypedEvent") {}
	};
	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(KeyCode key) : KeyEvent(key, EventType::EVENT_KEY_RELEASED, "KeyReleasedEvent") {}
	};
};