#pragma once

#include "stb_image/stb_image.h"
#include <glad/glad.h>
#include "VEngine/Core/Core.h"
#include "VEngine/Renderer/Model/Model.h"
#include "VEngine/Renderer/SSAO/SSAORenderer.h"

namespace VEngine
{
	class GBufferRenderer
	{
	public:
		GBufferRenderer();
		void Render(std::vector<Ref<Model>> models);

	private:
		void RenderScene(std::vector<Ref<Model>> models, Ref<Shader> shader);
		void RenderQuad();

		Ref<Texture2D> m_DiffuseTexture;
		Ref<Texture2D> m_SpecularTexture;

		unsigned int gBuffer;
		unsigned int gPosition, gNormal, gAlbedoSpec;
		Ref<ShaderLibrary> m_ShaderLibrary;

		Ref<SSAORenderer> m_SSAORenderer;
	};
}
