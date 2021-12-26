#pragma once

#include "stb_image/stb_image.h"
#include <glad/glad.h>
#include "VEngine/Core/Core.h"
#include "Model/Model.h"

namespace VEngine
{
	class ShadowMapRenderer
	{
	public:
		ShadowMapRenderer();
		void RenderShadowMap(std::vector<Ref<Model>> models);
		unsigned int GetDepthMapId() { return depthMap; }
		glm::mat4 GetLightSpaceMatrix() { return lightSpaceMatrix; }

	private:
		void RenderScene(std::vector<Ref<Model>> models, Ref<Shader> shader);
		void RenderQuad();

		Ref<ShaderLibrary> m_ShaderLibrary;
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		unsigned int depthMapFBO;
		unsigned int depthMap;
		glm::mat4 lightSpaceMatrix;
	};
}
