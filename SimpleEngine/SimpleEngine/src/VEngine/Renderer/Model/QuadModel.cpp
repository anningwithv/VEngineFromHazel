#include "QuadModel.h"

namespace VEngine
{
	QuadModel::QuadModel(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, const std::string & shaderName, const std::string& diffuseTex, const std::string& specularTex):Model(pos, scale, rotation, shaderName, diffuseTex, specularTex)
	{
		GLfloat vertices[] = {
			// positions            // texcoords  // normals         
			 25.0f, -0.5f,  25.0f,  25.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			-25.0f, -0.5f,  25.0f,   0.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			-25.0f, -0.5f, -25.0f,   0.0f, 25.0f,  0.0f, 1.0f, 0.0f,
									 			 
			 25.0f, -0.5f,  25.0f,  25.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			-25.0f, -0.5f, -25.0f,   0.0f, 25.0f,  0.0f, 1.0f, 0.0f,
			 25.0f, -0.5f, -25.0f,  25.0f, 10.0f,  0.0f, 1.0f, 0.0f,
		};										 

		//std::vector<GLuint> indices;
		unsigned int indices[6] = { 0,1,2, 3,4,5};

		std::vector<TextureData> textures;
		Vertex* vertexData = new Vertex(vertices, sizeof(vertices), indices, sizeof(indices) / sizeof(unsigned int));
		m_Mesh = new Mesh(this, vertexData, textures);

	}

	void QuadModel::Draw(TimeStep ts)
	{
		m_Material->Draw(m_Transform);

		m_Mesh->Draw(ts);

	}
}