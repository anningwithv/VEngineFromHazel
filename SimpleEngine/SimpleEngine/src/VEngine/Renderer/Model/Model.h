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

		Model(glm::vec3 pos, glm::vec3 scale);

		void Draw(TimeStep ts);
		void SetPosition(glm::vec3 position);

	private:
		Mesh* m_Mesh;
		//Material* m_Material;
		Texture* m_DiffuseTexture;
		Texture* m_SpecularTexture;

		glm::mat4 m_Transform;
	};
}
