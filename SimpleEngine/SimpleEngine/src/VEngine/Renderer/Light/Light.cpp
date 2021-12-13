#include "Light.h"

namespace VEngine
{
	VEngine::Light::Light(glm::vec3 color, glm::vec3 pos)
	{
		m_Color = color;
		m_Position = pos;
	}
}