#pragma once

#include "stb_image/stb_image.h"
#include <glad/glad.h>
#include "VEngine/Core/Core.h"
#include "VEngine/Renderer/Model/Model.h"

namespace VEngine
{
	class SSAORenderer
	{
	public:
		SSAORenderer(unsigned int gPositionBuffer, unsigned int gNormalBuffer);
		void Render();

	private:
		float Lerp(float a, float b, float f);
		//void RenderScene(std::vector<Ref<Model>> models, Ref<Shader> shader);
		void RenderQuad();

		unsigned int gPositionBuffer;
		unsigned int gNormalBuffer;
		unsigned int gNoiseTex;

		unsigned int ssaoFBO, ssaoBlurFBO;
		unsigned int ssaoColorBuffer, ssaoColorBufferBlur;

		std::vector<glm::vec3> ssaoKernel;
		std::vector<glm::vec3> ssaoNoise;

		Ref<ShaderLibrary> m_ShaderLibrary;
	};
}

