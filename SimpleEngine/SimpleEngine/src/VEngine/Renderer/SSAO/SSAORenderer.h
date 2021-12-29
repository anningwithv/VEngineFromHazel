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
		SSAORenderer(int gPositionBuffer, int gNormalBuffer, int gAlbedoBuffer);
		void RenderSSAO(std::vector<Ref<Model>> models);

	private:
		float Lerp(float a, float b, float f);
		//void RenderScene(std::vector<Ref<Model>> models, Ref<Shader> shader);
		void RenderQuad();

		int gPositionBuffer;
		int gNormalBuffer;
		int gNoiseTex;

		std::vector<glm::vec3> ssaoKernel;
		std::vector<glm::vec3> ssaoNoise;

		Ref<ShaderLibrary> m_ShaderLibrary;
	};
}

