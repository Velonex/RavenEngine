#pragma once
#include <Raven.h>

namespace rvn {

	class EditorCamera : public SceneCamera {
	public:
		EditorCamera();

		void onUpdate(Timestep ts);
		void onEvent(Event* e);
	
		void setPosition(const glm::vec3& position);
		void setRotation(float pitch, float yaw);

		glm::vec3 getPosition() const;
		float getPitch() const;
		float getYaw() const;
		glm::mat4 getTransform() const;
	private:
		glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
		float _pitch = 0.0f, _yaw = 0.0f;

	private:
		glm::vec2 _lastMousePos = { 0.0f, 0.0f };

	};

}