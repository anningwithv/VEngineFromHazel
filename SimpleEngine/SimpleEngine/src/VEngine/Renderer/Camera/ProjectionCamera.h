#pragma once

#include "Camera.h"

namespace VEngine
{
	class ProjectionCamera : public Camera
	{
	public:
		ProjectionCamera(glm::vec3 pos, float pitch, float yaw, glm::vec3 worldUp);

	protected:
		virtual void RecalculateViewMatrix() override;

	private:
		void CalculateVector();

	private:
		glm::vec3 m_TargetPos;
		glm::vec3 m_WorldUp;
	};
}
