#include "SkyboxModel.h"
#include "stb_image/stb_image.h"
#include "VEngine/Renderer/Material/SkyboxMaterial.h"

namespace VEngine
{
	SkyboxModel::SkyboxModel(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, const std::string & shaderName, const std::string& diffuseTex, const std::string& specularTex) 
		:Model(pos, scale, rotation, shaderName, diffuseTex, specularTex)
	{
		GLfloat vertices[] = {
			// Positions          // Texture Coords //Normal
			-1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

			 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

			-1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			-1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			-1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

			-1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			-1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

			-1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			-1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			 1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

		};

		unsigned int indices[36] = { 0,1,2, 3,4,5, 6,7,8, 9,10,11, 12,13,14, 15,16,17, 18,19,20, 21,22,23 , 24,25,26, 27,28,29, 30,31,32, 33,34,35 };

		std::vector<TextureData> textures;
		Vertex* vertexData = new Vertex(vertices, sizeof(vertices), indices, sizeof(indices) / sizeof(unsigned int));
		m_Mesh = new Mesh(this, vertexData, textures);

		m_Material = std::make_shared<SkyboxMaterial>(shaderName, diffuseTex, specularTex);
		
	}

	void SkyboxModel::Draw(TimeStep ts)
	{
		glDepthMask(GL_FALSE);// Remember to turn depth writing off

		m_Material->Draw(m_Transform);
		
		m_Mesh->Draw(ts);

		glDepthMask(GL_TRUE);
	}

	
}