#include "PBRMaterial.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "VEngine/Renderer/Renderer.h"

namespace VEngine
{
	PBRMaterial::PBRMaterial(const std::string & shaderName, const std::string & diffuseTex, const std::string & specularTex)
		:Material(shaderName, diffuseTex, specularTex)
	{
		// load PBR material textures
		// --------------------------
		m_AlbedoTexture = VEngine::Texture2D::Create("assets/textures/pbr/rusted_iron/albedo.png");
		m_NormalTexture = VEngine::Texture2D::Create("assets/textures/pbr/rusted_iron/normal.png");
		m_MetallicTexture = VEngine::Texture2D::Create("assets/textures/pbr/rusted_iron/metallic.png");
		m_RoughnessTexture = VEngine::Texture2D::Create("assets/textures/pbr/rusted_iron/roughness.png");
		m_AoTexture = VEngine::Texture2D::Create("assets/textures/pbr/rusted_iron/ao.png");
	}

	void PBRMaterial::Draw(glm::mat4 & transform)
	{
		auto shader = m_ShaderLibrary->Get(m_ShaderName);
		shader->Bind();

		m_AlbedoTexture->Bind(0);
		m_NormalTexture->Bind(1);
		m_MetallicTexture->Bind(2);
		m_RoughnessTexture->Bind(3);
		m_AoTexture->Bind(4);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", Renderer::s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("u_AmbientStrength", 1.5f);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("u_CameraPos", Renderer::s_SceneData->CameraPos);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("pointLights[0].color", Renderer::s_SceneData->LightColor);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("pointLights[0].position", Renderer::s_SceneData->LightPos);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("pointLights[0].constant", 1.0f);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("pointLights[0].linear", 1.0f);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("pointLights[0].quadratic", 1.0f);

		//Shadow 
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("shadowMap", 3);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("lightSpaceMatrix", Renderer::s_ShadowMapRenderer->GetLightSpaceMatrix());

	}


}