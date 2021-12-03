#include "Model.h"
#include <glad/glad.h>

namespace VEngine
{
	Model::Model(glm::vec3 pos, glm::vec3 scale)
	{
		m_Mesh = new Mesh({});

		SetPosition(pos);
		//m_Material = new Material(camera, projMat, pos, scale, vertShader, fragShader);

		//m_DiffuseTexture = new Texture("Image/wall.jpg");
		//m_SpecularTexture = new Texture("Image/smile.jpg");

	}

	void Model::Draw(TimeStep ts)
	{
		//m_Material->Draw();

		m_Mesh->Draw(ts);
	}

	void Model::SetPosition(glm::vec3 position)
	{
		m_Transform = glm::translate(glm::mat4(1.0f), position);
	}
}