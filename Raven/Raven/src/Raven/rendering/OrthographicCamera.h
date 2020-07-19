#pragma once
#include <glm.hpp>

namespace rvn {

	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void setProjection(float left, float right, float bottom, float top);

		void setPosition(const glm::vec3& position) { _position = position; _recalculateViewMatrix(); }
		const glm::vec3& getPosition() const { return _position; }

		void setRotation(float degrees) { _rotation = degrees; _recalculateViewMatrix(); }
		float getRotation() const { return _rotation; }

		const glm::mat4& getViewMatrix() const { return _viewMatrix; }
		const glm::mat4& getProjectionMatrix() const { return _projectionMatrix; }
		const glm::mat4& getViewProjectionMatrix() const { return _vpMatrix; }
	private:
		void _recalculateViewMatrix();
	private:
		glm::mat4 _viewMatrix;
		glm::mat4 _projectionMatrix;
		glm::mat4 _vpMatrix;

		glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
		float _rotation = 0.0f;
	};
}