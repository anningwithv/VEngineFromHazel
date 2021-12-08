#include "Model.h"
#include <glad/glad.h>

namespace VEngine
{
	Model::Model(glm::vec3 pos, glm::vec3 scale, const std::string& shaderName)
	{
		m_ShaderName = shaderName;

		Ref<Material> mat = std::make_shared<Material>(shaderName, "Box.jpg");

		m_Mesh = new Mesh(mat);

		SetPosition(pos);
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