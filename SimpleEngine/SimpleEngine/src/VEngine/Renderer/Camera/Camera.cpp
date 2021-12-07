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
			return new ProjectionCamera(glm::vec3(0.0f, 1.0f, 5.0f), 0.0f, -90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		default:
			break;
		}

		return nullptr;
	}
}