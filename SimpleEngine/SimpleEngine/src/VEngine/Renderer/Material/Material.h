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
		Material(const std::string& shaderName);
		void Draw(glm::mat4& transform);

	private:
		std::string m_ShaderName;
		Ref<ShaderLibrary> m_ShaderLibrary;
	};
}
