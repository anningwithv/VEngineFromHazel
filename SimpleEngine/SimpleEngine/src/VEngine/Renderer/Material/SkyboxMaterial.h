#pragma once

#include "stb_image/stb_image.h"
#include <glad/glad.h>
#include "VEngine/Core/Core.h"

#include "Material.h"

namespace VEngine
{
	class SkyboxMaterial : public Material
	{
	public:
		SkyboxMaterial(const std::string& shaderName, const std::string& diffuseTex, const std::string& specularTex);
		virtual void Draw(glm::mat4& transform) override;

	private:
		GLuint LoadCubemap(std::vector<const GLchar*> faces);
		GLuint m_CubemapTexture;

		unsigned int m_skyboxVAO;
		unsigned int m_skyboxVBO;
	};
}
