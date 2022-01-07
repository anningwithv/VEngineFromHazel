#pragma once

#include "stb_image/stb_image.h"
#include <glad/glad.h>
#include "VEngine/Core/Core.h"

#include "Material.h"

namespace VEngine
{
	class PBRMaterial : public Material
	{
	public:
		PBRMaterial(const std::string& shaderName, const std::string& diffuseTex, const std::string& specularTex);
		virtual void Draw(glm::mat4& transform) override;

	private:
		Ref<Texture2D> m_AlbedoTexture;
		Ref<Texture2D> m_NormalTexture;
		Ref<Texture2D> m_MetallicTexture;
		Ref<Texture2D> m_RoughnessTexture;
		Ref<Texture2D> m_AoTexture;
	};
}
