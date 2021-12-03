#pragma once

#include "VEngine/Renderer/Shader/Shader.h"
#include "VEngine/Renderer/Camera/Camera.h"
#include "VEngine/Renderer/Texture/Texture.h"

namespace VEngine
{
	class Material
	{
	public:
		Material(Camera* camera, glm::mat4 projMat, glm::vec3 pos, glm::vec3 scale, const char* vertShader, const char* fragShader);
		void Draw();

	private:
		Camera* m_Camera;
		Shader* m_Shader;
		Texture* m_DiffuseTexture;
		Texture* m_SpecularTexture;
	};
}
