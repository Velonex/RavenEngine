#include <pch.h>
#include "Entity.h"

namespace rvn {
	Entity::Entity(entt::entity handle, Scene* scene)
		: _handle(handle), _scene(scene)
	{
	}
}