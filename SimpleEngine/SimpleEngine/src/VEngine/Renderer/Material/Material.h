#pragma once

#include "VEngine/Renderer/Shader/Shader.h"
#include "VEngine/Renderer/Camera/Camera.h"
#include "VEngine/Renderer/Texture/Texture.h"
#include <glm/glm.hpp>

namespace VEngine
{
	class Material
	{
	public:
		Material(const std::string& shaderName, const std::string& diffuseTex, const std::string& specularTex);
		virtual void Draw(glm::mat4& transform);

	protected:
		std::string m_ShaderName;
		Ref<ShaderLibrary> m_ShaderLibrary;

		Ref<Texture2D> m_DiffuseTexture;
		Ref<Texture2D> m_SpecularTexture;
	};
}
