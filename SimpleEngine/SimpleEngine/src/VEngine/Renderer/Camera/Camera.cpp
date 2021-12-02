#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "VEngine/GameMode.h"
#include "OrthographicCamera.h"
#include "ProjectionCamera.h"

namespace VEngine
{
	Camera::Camera()
	{
	}

	Camera * Camera::Create()
	{
		switch (GameMode::GetGameMode())
		{
		case GameMode::Mode::D2:
			return new OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
		case GameMode::Mode::D3:
			return new ProjectionCamera();
		default:
			break;
		}

		return nullptr;
	}

	void Camera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)
			/** glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1))*/;
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}