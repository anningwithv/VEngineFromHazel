#pragma once

#include "Mesh.h"
#include "VEngine/Renderer/Shader/Shader.h"
#include "VEngine/Renderer/Camera/Camera.h"
#include "VEngine/Renderer/Texture/Texture.h"

namespace VEngine
{
	class Model
	{
	public:

		Model(Camera* camera, glm::mat4 projMat, glm::vec3 pos, glm::vec3 scale, const char* vertShader, const char* fragShader);

		void Draw();

	private:
		Camera* m_Camera;
		Mesh* m_Mesh;
		//Material* m_Material;
		Texture* m_DiffuseTexture;
		Texture* m_SpecularTexture;
	};
}
