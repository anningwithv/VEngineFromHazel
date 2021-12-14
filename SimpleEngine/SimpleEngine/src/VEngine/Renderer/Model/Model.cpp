#include "Model.h"
#include "Mesh.h"
#include <glad/glad.h>

namespace VEngine
{
	Model::Model(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, const std::string& shaderName)
	{
		m_ShaderName = shaderName;

		Ref<Material> mat = std::make_shared<Material>(shaderName, "Box.jpg");

		std::vector<Vertex> vertexs;
		m_Mesh = new Mesh(this, mat, vertexs);

		m_Position = pos;
		m_Rotation = rotation;
		m_Scale = scale;

		RefreshTranform();
	}

	void Model::Draw(TimeStep ts)
	{
		//m_Material->Draw();

		m_Mesh->Draw(ts);
	}

	void Model::SetPosition(glm::vec3 position)
	{
		m_Position = position;
		RefreshTranform();
	}

	void Model::SetRotation(glm::vec3 rotation)
	{
		m_Rotation = rotation;
		RefreshTranform();
	}

	void Model::SetScale(glm::vec3 scale)
	{
		m_Scale = scale;
		RefreshTranform();
	}

	void Model::RefreshTranform()
	{
		m_Transform = glm::translate(glm::mat4(1.0f), m_Position)
			*glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1.0, 0.0, 0.0))
			*glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0.0, 1.0, 0.0))
			*glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0.0, 0.0, 1.0))
			* glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale.x, m_Scale.y, m_Scale.z));
	}

	glm::mat4 Model::GetTransform()
	{
		return m_Transform;
	}
}