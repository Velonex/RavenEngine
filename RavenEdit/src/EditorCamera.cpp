#include "EditorCamera.h"

namespace rvn {

	static const glm::vec3 forwardVector = { 0.0f, 0.0f, -1.0f };
	static const glm::vec3 upwardVector = { 0.0f, 1.0f, 0.0f };

	EditorCamera::EditorCamera()
	{
		this->setProjectionType(ProjectionType::Perspective);
	}
	void EditorCamera::onUpdate(Timestep ts)
	{
		// Reset camera
		if (Input::isKeyPressed(Key::KP7) || Input::isKeyPressed(Key::Home)) {
			_position = glm::vec3(0.0f);
			_pitch = _yaw = 0.0f;
		}

		glm::vec2 currentMousePos = Input::getMousePos();
		glm::vec2 mouseDelta = currentMousePos - _lastMousePos;
		
		if (Input::isMousePressed(Mouse::ButtonRight)) {
			_yaw -= (mouseDelta.x / this->_width);
			_pitch -= (mouseDelta.y / this->_height);
		}

		_lastMousePos = Input::getMousePos();

		static constexpr float _90degInRad = glm::radians(90.0f);

		_pitch = glm::max(-_90degInRad, _pitch);
		_pitch = glm::min(_90degInRad, _pitch);

		// Update position

		glm::quat rotXZ = glm::quat({ 0.0f, _yaw + _90degInRad, 0.0f });
		glm::vec3 movementVectorXZ = glm::rotate(rotXZ, forwardVector);
		
		glm::quat rotXYZ = glm::quat({ _pitch, _yaw, 0.0f });
		glm::vec3 movementVectorXYZ = glm::rotate(rotXYZ, upwardVector);

		if (Input::isMousePressed(Mouse::ButtonMiddle)) {
			_position += (mouseDelta.x / this->_width) * movementVectorXZ * 2.f;
			_position += (mouseDelta.y / this->_height) * movementVectorXYZ * 2.f;
		}
	}

	void EditorCamera::onEvent(Event* e)
	{
		switch (e->getType())
		{
		case EventType::EVENT_MOUSE_SCROLLED: {
			MouseScrolledEvent* ev = (MouseScrolledEvent*)e;
			glm::vec3 movementVec = glm::rotate(glm::quat({ _pitch, _yaw, 0.0f }), forwardVector);
			_position += movementVec * ev->getYOffset();
			break;
		}
		default:
			break;
		}
	}

	void EditorCamera::setPosition(const glm::vec3& position)
	{
		_position = position;
	}

	void EditorCamera::setRotation(float pitch, float yaw)
	{
		_pitch = pitch;
		_yaw = yaw;
	}

	glm::vec3 EditorCamera::getPosition() const
	{
		return _position;
	}

	float EditorCamera::getPitch() const
	{
		return _pitch;
	}

	float EditorCamera::getYaw() const
	{
		return _yaw;
	}

	glm::mat4 EditorCamera::getTransform() const
	{
		glm::mat4 rot = glm::toMat4(glm::quat({ _pitch, _yaw, 0.0f }));


		return glm::translate(glm::mat4(1.0f), _position) * rot;
	}

}