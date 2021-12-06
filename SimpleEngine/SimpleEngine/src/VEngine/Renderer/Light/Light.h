#pragma once

#include "glm/glm.hpp"

namespace VEngine
{
	class Light
	{
	public:
		Light(glm::vec3 color, glm::vec3 pos);

		glm::vec3 GetLightColor() const { return m_Color; };
		glm::vec3 GetLightPosition() const { return m_Position; };

	private:
		glm::vec3 m_Color;
		glm::vec3 m_Position;
	};
}
