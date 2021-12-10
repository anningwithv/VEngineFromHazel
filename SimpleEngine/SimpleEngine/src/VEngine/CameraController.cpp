#include "CameraController.h"

#include "VEngine/Input.h"
#include "VEngine/KeyCodes.h"
#include "VEngine/Log.h"

namespace VEngine
{
	CameraController::CameraController(Camera* camera, float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio)
	{
		m_Camera = camera;
		m_CameraPosition = camera->GetPosition();
		m_CameraRotation = camera->GetRotation();

		//m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	}

	void CameraController::OnUpdate(TimeStep ts)
	{
		if (Input::IsKeyPressed(VENGINE_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(VENGINE_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(VENGINE_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(VENGINE_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(VENGINE_KEY_Q))
				m_CameraRotation.y += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(VENGINE_KEY_E))
				m_CameraRotation.y -= m_CameraRotationSpeed * ts;

			m_Camera->SetRotation(m_CameraRotation);
		}

		VENGINE_CORE_INFO("Camera position is: x:{0} y:{1} z:{2}", m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z);
		VENGINE_CORE_INFO("Camera rotation is: {0}", m_CameraRotation.y);

		m_Camera->SetPosition(m_CameraPosition);

		//m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(VENGINE_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(VENGINE_BIND_EVENT_FN(CameraController::OnWindowResized));
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		if (dynamic_cast<OrthographicCamera*>(m_Camera) != nullptr)
		{
			dynamic_cast<OrthographicCamera*>(m_Camera)->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		}
		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();

		if (dynamic_cast<OrthographicCamera*>(m_Camera) != nullptr)
		{
			dynamic_cast<OrthographicCamera*>(m_Camera)->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		}
		
		return false;
	}

}