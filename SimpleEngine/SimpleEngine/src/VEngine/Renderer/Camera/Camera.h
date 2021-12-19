#pragma once

#include <glm/glm.hpp>

namespace VEngine
{
	class Camera
	{
	public:
		Camera();

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3 pos) { m_Position = pos; RecalculateViewMatrix();}
		glm::vec3 GetRotation() const { return m_Rotation; }
		void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		glm::vec3 GetCameraForward() { return m_CameraForward; }
		glm::vec3 GetCameraUp() { return m_CameraUp; }
		glm::vec3 GetCameraRight() { return m_CameraRight; }

		static Camera* Create();

	protected:
		virtual void RecalculateViewMatrix() = 0;


	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;

		glm::vec3 m_Rotation;

		glm::vec3 m_CameraUp;
		glm::vec3 m_CameraForward;
		glm::vec3 m_CameraRight;
	};
}
