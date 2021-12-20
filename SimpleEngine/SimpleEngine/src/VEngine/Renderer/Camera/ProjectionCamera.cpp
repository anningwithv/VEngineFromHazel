#include "ProjectionCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace VEngine
{
	VEngine::ProjectionCamera::ProjectionCamera(glm::vec3 pos, float pitch, float yaw, glm::vec3 worldUp)
	{
		m_Position = pos;

		m_Rotation = glm::vec3(pitch, yaw, 0.0f);

		m_WorldUp = worldUp;

		RecalculateViewMatrix();
	}

	void ProjectionCamera::RecalculateViewMatrix()
	{
		CalculateVector();
	}

	void ProjectionCamera::CalculateVector()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
		front.y = sin(glm::radians(m_Rotation.x));
		front.z = sin(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));

		m_CameraForward = glm::normalize(front);
		m_CameraRight = glm::normalize(glm::cross(m_WorldUp, m_CameraForward));
		m_CameraUp = glm::normalize(glm::cross(m_CameraForward, m_CameraRight));

		m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)1280.0f / (float)720.0f, 0.1f, 100.0f);
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_CameraForward, m_CameraUp);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}