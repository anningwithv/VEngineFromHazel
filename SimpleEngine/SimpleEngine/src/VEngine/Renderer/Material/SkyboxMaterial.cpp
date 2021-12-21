#include "SkyboxMaterial.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "VEngine/Renderer/Renderer.h"

namespace VEngine
{
	SkyboxMaterial::SkyboxMaterial(const std::string & shaderName, const std::string & diffuseTex, const std::string & specularTex)
		:Material(shaderName, diffuseTex, specularTex)
	{
		// Cubemap (Skybox)
		std::vector<const GLchar*> faces;
		faces.push_back("assets/skybox/right.jpg");
		faces.push_back("assets/skybox/left.jpg");
		faces.push_back("assets/skybox/top.jpg");
		faces.push_back("assets/skybox/bottom.jpg");
		faces.push_back("assets/skybox/front.jpg");
		faces.push_back("assets/skybox/back.jpg");
		m_CubemapTexture = LoadCubemap(faces);

		////准备数据
		//float skyboxVertices[] = {
		//	// positions          
		//	-1.0f,  1.0f, -1.0f,
		//	-1.0f, -1.0f, -1.0f,
		//	 1.0f, -1.0f, -1.0f,
		//	 1.0f, -1.0f, -1.0f,
		//	 1.0f,  1.0f, -1.0f,
		//	-1.0f,  1.0f, -1.0f,

		//	-1.0f, -1.0f,  1.0f,
		//	-1.0f, -1.0f, -1.0f,
		//	-1.0f,  1.0f, -1.0f,
		//	-1.0f,  1.0f, -1.0f,
		//	-1.0f,  1.0f,  1.0f,
		//	-1.0f, -1.0f,  1.0f,

		//	 1.0f, -1.0f, -1.0f,
		//	 1.0f, -1.0f,  1.0f,
		//	 1.0f,  1.0f,  1.0f,
		//	 1.0f,  1.0f,  1.0f,
		//	 1.0f,  1.0f, -1.0f,
		//	 1.0f, -1.0f, -1.0f,

		//	-1.0f, -1.0f,  1.0f,
		//	-1.0f,  1.0f,  1.0f,
		//	 1.0f,  1.0f,  1.0f,
		//	 1.0f,  1.0f,  1.0f,
		//	 1.0f, -1.0f,  1.0f,
		//	-1.0f, -1.0f,  1.0f,

		//	-1.0f,  1.0f, -1.0f,
		//	 1.0f,  1.0f, -1.0f,
		//	 1.0f,  1.0f,  1.0f,
		//	 1.0f,  1.0f,  1.0f,
		//	-1.0f,  1.0f,  1.0f,
		//	-1.0f,  1.0f, -1.0f,

		//	-1.0f, -1.0f, -1.0f,
		//	-1.0f, -1.0f,  1.0f,
		//	 1.0f, -1.0f, -1.0f,
		//	 1.0f, -1.0f, -1.0f,
		//	-1.0f, -1.0f,  1.0f,
		//	 1.0f, -1.0f,  1.0f
		//};

		//glGenVertexArrays(1, &m_skyboxVAO);
		//glGenBuffers(1, &m_skyboxVBO);
		//glBindVertexArray(m_skyboxVAO);
		//glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	}

	void SkyboxMaterial::Draw(glm::mat4 & transform)
	{
		auto shader = m_ShaderLibrary->Get(m_ShaderName);
		shader->Bind();
		// Draw skybox first

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("projection", Renderer::s_SceneData->ProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("view", glm::mat4(glm::mat3(Renderer::s_SceneData->ViewMatrix)));

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);

		//glBindVertexArray(m_skyboxVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);
	}

	GLuint SkyboxMaterial::LoadCubemap(std::vector<const GLchar*> faces)
	{
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, channels;
		//unsigned char* image;

		for (GLuint i = 0; i < faces.size(); i++)
		{
			//OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部
			stbi_set_flip_vertically_on_load(1);

			stbi_uc* data = stbi_load(faces[i], &width, &height, &channels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				VENGINE_ERROR("Cubemap texture load failed");
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return textureID;
	}
}