#pragma once

#include <vector>
#include <string>
#include <Raven_Core/core/Logger.h>

namespace rvn {
	enum class EventType {
		ALL,
		UNINITIALIZED,
		EVENT_WINDOW_CLOSE, EVENT_WINDOW_RESIZE, // Window events
		EVENT_KEY_PRESSED, EVENT_KEY_RELEASED, EVENT_KEY_TYPED, // Keyboard events
		EVENT_MOUSE_MOVED, EVENT_MOUSE_SCROLLED, EVENT_MOUSE_BUTTON_PRESSED, EVENT_MOUSE_BUTTON_RELEASED //Mouse events
	};
	class Event {
	public:
		Event(EventType type, std::string name) {
			this->_name = name;
			this->_type = type;
		}
		std::string getName() { return _name; }
		EventType getType() { return _type; }
		bool handled = false;
	private:
		std::string _name;
		EventType _type;
	};
	struct EventListener {
		virtual void onEvent(Event* e) = 0;
	};
	class EventHandler {
	public:
		void subscribe(EventListener* clazz, EventType type) {
			if (std::find(_subs.begin(), _subs.end(), _Subscriber(clazz, type)) != _subs.end()) {
				LOG_ENGINE_WARN("Instance has subscribed multiple times");
				return;
			}
			_subs.push_back({ clazz, type });
		}
		// The event will get dispatched and will automatically be deleted
		void dispatchEvent(Event* e) {
			for (int i = 0; i < _subs.size(); i++) {
				if ((e->getType() == _subs[i].type || _subs[i].type == rvn::EventType::ALL) && e->getType() != rvn::EventType::UNINITIALIZED) {
					_subs[i].clazz->onEvent(e);
				}
				if (e->handled) break;
			}
			delete e;
		}
		void unsubscribe(EventListener* clazz, EventType type) {
			if (!(std::find(_subs.begin(), _subs.end(), _Subscriber(clazz, type)) != _subs.end())) {
				LOG_ENGINE_WARN("Instance that hasn't subscribed tried to unsubscribe");
				return;
			}
			_subs.erase(std::find(_subs.begin(), _subs.end(), _Subscriber(clazz, type)));
		}
	private:
		struct _Subscriber {
			EventListener* clazz;
			EventType type;
			bool operator==(_Subscriber rhs) {
				return rhs.clazz == clazz && rhs.type == type;
			}
			_Subscriber(EventListener* clazz, EventType type) : clazz(clazz), type(type) {

			}
			_Subscriber() : clazz(nullptr), type(EventType::UNINITIALIZED) {

			}
		};
		std::vector<_Subscriber> _subs;
	};
}