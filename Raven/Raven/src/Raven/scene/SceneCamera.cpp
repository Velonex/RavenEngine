#include <pch.h>
#include "SceneCamera.h"
#include <gtc/matrix_transform.hpp>

namespace rvn {
	SceneCamera::SceneCamera()
	{
		recalculateProjection();
	}
	void SceneCamera::setPerspective(float verticalFOV, float nearClip, float farClip)
	{
		_projectionType = ProjectionType::Perspective;
		_perspectiveFOV = verticalFOV;
		_perspectiveNear = nearClip;
		_perspectiveFar = farClip;
		recalculateProjection();
	}
	void SceneCamera::setOrthographic(float size, float nearClip, float farClip)
	{
		_projectionType = ProjectionType::Orthographic;
		_orthographicSize = size;
		_orthographicNear = nearClip;
		_orthographicFar = farClip;
		recalculateProjection();
	}
	void SceneCamera::setViewportSize(std::uint32_t width, std::uint32_t height)
	{
		_aspectRatio = (float)width / (float)height;
		recalculateProjection();
	}
	void SceneCamera::recalculateProjection()
	{
		if (_projectionType == ProjectionType::Perspective) {
			_projection = glm::perspective(_perspectiveFOV, _aspectRatio, _perspectiveNear, _perspectiveFar);
		} else
		{
			float left = -_orthographicSize * _aspectRatio * 0.5f;
			float right = _orthographicSize * _aspectRatio * 0.5f;
			float bottom = -_orthographicSize * 0.5f;
			float top = _orthographicSize * 0.5f;

			_projection = glm::ortho(left, right, bottom, top, _orthographicNear, _orthographicFar);
		}
	}
}