#include "Model.h"
#include "Mesh.h"
#include <glad/glad.h>

namespace VEngine
{
	Model::Model(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, const std::string& shaderName)
	{
		m_ShaderName = shaderName;

		m_Material = std::make_shared<Material>(shaderName, "Box.jpg", "Box_specular.jpg");


		GLfloat vertices[] = {
			// Positions          // Texture Coords //Normal
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

		};

		//std::vector<GLuint> indices;
		unsigned int indices[36] = { 0,1,2, 3,4,5, 6,7,8, 9,10,11, 12,13,14, 15,16,17, 18,19,20, 21,22,23 , 24,25,26, 27,28,29, 30,31,32, 33,34,35 };

		std::vector<TextureData> textures;
		Vertex* vertexData = new Vertex(vertices, sizeof(vertices), indices, sizeof(indices) / sizeof(unsigned int));
		m_Mesh = new Mesh(this, vertexData, textures);

		m_Position = pos;
		m_Rotation = rotation;
		m_Scale = scale;

		RefreshTranform();
	}

	void Model::Draw(TimeStep ts)
	{
		m_Material->Draw(m_Transform);

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