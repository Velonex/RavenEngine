#include <pch.h>
#include "OrthographicCamera.h"
#include <gtc/matrix_transform.hpp>

namespace rvn {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: _projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), _viewMatrix(1.0f)
	{
		_vpMatrix = _projectionMatrix * _viewMatrix;
	}
	void OrthographicCamera::setProjection(float left, float right, float bottom, float top)
	{
		_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

		_vpMatrix = _projectionMatrix * _viewMatrix;
	}
	void OrthographicCamera::_recalculateViewMatrix()
	{
		_viewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), _position) * glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), { 0, 0, 1 }));
		
		_vpMatrix = _projectionMatrix * _viewMatrix;
	}
}