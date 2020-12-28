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

	private:
		void recalculateProjection();
	private:
		ProjectionType _projectionType = ProjectionType::Orthographic;

		float _perspectiveFOV = glm::radians(45.0f);
		float _perspectiveNear = 0.01f, _perspectiveFar = 10000.0f;

		float _orthographicSize = 10.0f;
		float _orthographicNear = -1.0f, _orthographicFar = 1.0f;

		float _aspectRatio = 0.0f;
	};
}