#include "Material.h"

namespace VEngine
{
	Material::Material(Camera* camera, glm::mat4 projMat, glm::vec3 pos, glm::vec3 scale, const char* vertShader, const char* fragShader)
	{
		m_Camera = camera;

		//m_Shader = new Shader(vertShader, fragShader);

		//// 设置shader的Uniform之前需要先调用Use
		//m_Shader->use();

		//// 设置模型矩阵
		//glm::mat4 modelMat;
		//modelMat = glm::translate(modelMat, pos);
		//modelMat = glm::scale(modelMat, scale);
		//m_Shader->setMatrix("modelMat", modelMat);

		//// 设置投影矩阵
		//m_Shader->setMatrix("projMat", projMat);

		//// 设置贴图绑定的寄存器index
		//m_Shader->setInt("material.diffuse", 0);
		//m_Shader->setInt("material.specular", 1);

		//// 设置光照
		//m_Shader->setVec3("lightPos", glm::vec3(2, 2, -1));
		////m_Shader->setVec3("lightColor", glm::vec3(1, 1, 1));
		//m_Shader->setVec3("objColor", glm::vec3(1, 1, 1));

		//m_Shader->setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		//m_Shader->setVec3("light.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));
		//m_Shader->setVec3("light.specular", glm::vec3(1, 1, 1));

		//m_Shader->setVec3("material.ambient", glm::vec3(1));
		////m_Shader->setVec3("material.diffuse", glm::vec3(1, 0, 0));
		////m_Shader->setVec3("material.specular", glm::vec3(0, 0, 1));
		//m_Shader->setFloat("material.shinness", 32);

		//// 贴图
		//m_DiffuseTexture = new Texture("Image/wall.jpg");
		//m_SpecularTexture = new Texture("Image/container2_specular.jpg");
		//// Bind texture
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m_DiffuseTexture->GetTextureID());
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, m_SpecularTexture->GetTextureID());

	}

	void Material::Draw()
	{
		//m_Shader->use();

		//m_Shader->setVec3("viewPos", m_Camera->GetPosition());
		//m_Shader->setMatrix("viewMat", m_Camera->GetViewMatrix());
	}
}