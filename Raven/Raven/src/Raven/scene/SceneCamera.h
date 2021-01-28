#pragma once

#include <Raven/rendering/Camera.h>

namespace rvn {

	class SceneCamera : public Camera {
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void setPerspective(float verticalFOV, float nearClip, float farClip);
		void setOrthographic(float size, float nearClip, float farClip);

		void setViewportSize(std::uint32_t width, std::uint32_t height);

		ProjectionType getProjectionType() const { return _projectionType; }
		void setProjectionType(ProjectionType type) { _projectionType = type; recalculateProjection(); }

		// Orthographic
		float getOrthographicSize() const { return _orthographicSize; };
		float getOrthographicNearClip() const { return _orthographicNear; };
		float getOrthographicFarClip() const { return _orthographicFar; };

		void setOrthographicSize(float size) { _orthographicSize = size; recalculateProjection(); };
		void setOrthographicNearClip(float nearClip) { _orthographicNear = nearClip; recalculateProjection(); };
		void setOrthographicFarClip(float farClip) { _orthographicFar = farClip; recalculateProjection(); };

		// Perspective
		float getPerspectiveFOV() const { return _perspectiveFOV; };
		float getPerspectiveNearClip() const { return _perspectiveNear; };
		float getPerspectiveFarClip() const { return _perspectiveFar; };

		void setPerspectiveFOV(float fov) { _perspectiveFOV = fov; recalculateProjection(); };
		void setPerspectiveNearClip(float nearClip) { _perspectiveNear = nearClip; recalculateProjection(); };
		void setPerspectiveFarClip(float farClip) { _perspectiveFar = farClip; recalculateProjection(); };
	private:
		void recalculateProjection();
	private:
		ProjectionType _projectionType = ProjectionType::Orthographic;

		float _perspectiveFOV = glm::radians(45.0f);
		float _perspectiveNear = 0.01f, _perspectiveFar = 10000.0f;

		float _orthographicSize = 10.0f;
		float _orthographicNear = -1.0f, _orthographicFar = 1.0f;

		float _aspectRatio = 16.f/9.f;
	};
}