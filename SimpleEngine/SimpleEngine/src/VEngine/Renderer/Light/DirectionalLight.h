#pragma once

#include "Light.h"

namespace VEngine
{
	class DirectionalLight : public Light
	{
	public:
		glm::vec3 GetDirection() const { return m_Direction; }
		void SetDirection(glm::vec3 dir) { m_Direction = dir; }

	private:
		glm::vec3 m_Direction;
	};
}
