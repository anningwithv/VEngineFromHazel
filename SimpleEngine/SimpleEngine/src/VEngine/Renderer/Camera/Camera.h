#pragma once

#include <glm/glm.hpp>

namespace VEngine
{
	class Camera
	{
	public:
		Camera();

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3 pos) { m_Position = pos; }
		float GetRotation() const { return m_Rotation; }
		void SetRotation(const float rotation) { m_Rotation = rotation; }
		
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		static Camera* Create();

	protected:
		void RecalculateViewMatrix();

	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;
		float m_Rotation = 0.0f;
	};
}
