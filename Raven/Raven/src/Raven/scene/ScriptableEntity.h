#pragma once

#include "Entity.h"

namespace rvn {
	class ScriptableEntity {
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& getComponent() {
			return _entity.getComponent<T>();
		}
		template<typename T>
		bool hasComponent() {
			return _entity.hasComponent<T>();
		}
	protected:
		virtual void onCreate() {}
		virtual void onDestroy() {}
		virtual void onUpdate(Timestep ts) {}
	private:
		Entity _entity;
		friend class Scene;
		friend struct NativeScriptComponent;
	};
}