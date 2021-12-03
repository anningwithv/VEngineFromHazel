#include "Model.h"
#include <glad/glad.h>

namespace VEngine
{
	Model::Model(Camera* camera, glm::mat4 projMat, glm::vec3 pos, glm::vec3 scale, const char* vertShader, const char* fragShader)
	{
		m_Camera = camera;

		m_Mesh = new Mesh({});
		//m_Material = new Material(camera, projMat, pos, scale, vertShader, fragShader);

		//m_DiffuseTexture = new Texture("Image/wall.jpg");
		//m_SpecularTexture = new Texture("Image/smile.jpg");

	}

	void Model::Draw()
	{
		//m_Material->Draw();

		m_Mesh->Draw();
	}
}