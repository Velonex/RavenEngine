#pragma once
#include <Raven/rendering/OrthographicCamera.h>
#include <glm.hpp>
#include <Raven/application/Timestep.h>
#include <Raven/events/WindowEvents.h>
#include <Raven/events/MouseEvents.h>

namespace rvn {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool allowRotation = false);

		void onUpdate(Timestep ts);
		void onEvent(Event* e);

		OrthographicCamera& getCamera() { return _camera; }
		const OrthographicCamera& getCamera() const { return _camera; }

		void setZoomLevel(float zoomLevel) { _zoomLevel = zoomLevel; }
		float getZoomLevel() const { return _zoomLevel; }
	private:
		void onWindowResize(WindowResizeEvent* e);
		void onMouseScrolled(MouseScrolledEvent* e);
	private:
		bool _allowRotation;
		float _aspectRatio;

		float _zoomLevel = 1.0f;
		glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
		float _rotation = 0.0f;
		float _rotationSpeed = 180.0f, _translationSpeed = 1.0f;

		OrthographicCamera _camera;
	};

}