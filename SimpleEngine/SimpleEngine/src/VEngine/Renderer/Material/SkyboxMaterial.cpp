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
		faces.push_back("assets/skybox/back.jpg");
		faces.push_back("assets/skybox/front.jpg");
		m_CubemapTexture = LoadCubemap(faces);
	}

	void SkyboxMaterial::Draw(glm::mat4 & transform)
	{
		auto shader = m_ShaderLibrary->Get(m_ShaderName);
		shader->Bind();
		// Draw skybox first

		//glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", Renderer::s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		// skybox cube
		//glBindVertexArray(skyboxVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glUniform1i(glGetUniformLocation(shader.Program, "skybox"), 0);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("skybox", 3);
		glBindTexture(3/*GL_TEXTURE_CUBE_MAP*/, m_CubemapTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);

	}

	GLuint SkyboxMaterial::LoadCubemap(std::vector<const GLchar*> faces)
	{
		GLuint textureID;
		glGenTextures(1, &textureID);

		int width, height, channels;
		//unsigned char* image;

		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		for (GLuint i = 0; i < faces.size(); i++)
		{
			//OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部
			stbi_set_flip_vertically_on_load(1);

			stbi_uc* data = stbi_load(faces[i], &width, &height, &channels, 0);
			//image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			//SOIL_free_image_data(image);
			stbi_image_free(data);
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