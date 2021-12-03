#pragma once

#include <glm/glm.hpp>
#include "Camera.h"

namespace VEngine
{
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		void SetProjection(float left, float right, float bottom, float top);

	protected:
		virtual void RecalculateViewMatrix() override;
	};
}
