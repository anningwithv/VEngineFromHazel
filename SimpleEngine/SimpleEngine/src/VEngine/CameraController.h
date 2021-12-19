#pragma once

#include "VEngine/Renderer/Camera/OrthographicCamera.h"
#include "VEngine/Renderer/Camera/Camera.h"
#include "VEngine/Core/Timestep.h"
#include "VEngine/Events/ApplicationEvent.h"
#include "VEngine/Events/MouseEvent.h"

namespace VEngine {

	class CameraController
	{
	public:
		CameraController(Camera* camera, float aspectRatio, bool rotation = false);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		Camera* GetCamera() { return m_Camera; }
		const Camera* GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		Camera* m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_CameraRotation = { 0.0f, 0.0f, 0.0f };

		float m_CameraTranslationSpeed = 0.05f;
		float m_CameraRotationSpeed = 0.1f;
	};

}