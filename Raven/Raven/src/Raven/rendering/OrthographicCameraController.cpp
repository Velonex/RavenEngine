#include <pch.h>
#include "OrthographicCameraController.h"
#include <Raven/application/Input.h>

namespace rvn {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool allowRotation)
		: _allowRotation(allowRotation), _aspectRatio(aspectRatio), _camera(-aspectRatio * _zoomLevel, aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel)
	{
	}

	void OrthographicCameraController::onUpdate(Timestep ts)
	{
		if (Input::isKeyPressed(KEY_A))
		{
			_position.x -= cos(glm::radians(_rotation)) * _translationSpeed * ts;
			_position.y -= sin(glm::radians(_rotation)) * _translationSpeed * ts;
		}
		else if (Input::isKeyPressed(KEY_D))
		{
			_position.x += cos(glm::radians(_rotation)) * _translationSpeed * ts;
			_position.y += sin(glm::radians(_rotation)) * _translationSpeed * ts;
		}

		if (Input::isKeyPressed(KEY_W))
		{
			_position.x += -sin(glm::radians(_rotation)) * _translationSpeed * ts;
			_position.y += cos(glm::radians(_rotation)) * _translationSpeed * ts;
		}
		else if (Input::isKeyPressed(KEY_S))
		{
			_position.x -= -sin(glm::radians(_rotation)) * _translationSpeed * ts;
			_position.y -= cos(glm::radians(_rotation)) * _translationSpeed * ts;
		}

		if (_allowRotation)
		{
			if (Input::isKeyPressed(KEY_Q))
				_rotation += _rotationSpeed * ts;
			if (Input::isKeyPressed(KEY_E))
				_rotation -= _rotationSpeed * ts;

			if (_rotation > 180.0f)
				_rotation -= 360.0f;
			else if (_rotation <= -180.0f)
				_rotation += 360.0f;

			_camera.setRotation(_rotation);
		}

		_camera.setPosition(_position);

	}

	void OrthographicCameraController::onEvent(Event* e)
	{
		switch (e->getType()) {
		case EventType::EVENT_WINDOW_RESIZE: {
			onWindowResize((WindowResizeEvent*)e);
			break;
		}
		case EventType::EVENT_MOUSE_SCROLLED: {
			onMouseScrolled((MouseScrolledEvent*)e);
			break;
		}
		}
	}

	void OrthographicCameraController::onWindowResize(WindowResizeEvent* e)
	{
		_aspectRatio = (float)e->getWidth() / (float)e->getHeight();
		_camera.setProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
	}

	void OrthographicCameraController::onMouseScrolled(MouseScrolledEvent* e)
	{
		_translationSpeed = _zoomLevel;
		_zoomLevel -= e->getYOffset() * 0.25f;
		_zoomLevel = (_zoomLevel < 0.25f) ? 0.25f : _zoomLevel;
		_camera.setProjection(-_aspectRatio * _zoomLevel, _aspectRatio * _zoomLevel, -_zoomLevel, _zoomLevel);
	}

}