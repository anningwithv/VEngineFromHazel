#include "Material.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "VEngine/Renderer/Renderer.h"

namespace VEngine
{
	Material::Material(const std::string& shaderName, const std::string& diffuseTex, const std::string& specularTex)
	{
		m_ShaderName = shaderName;

		m_ShaderLibrary = std::make_shared<ShaderLibrary>();
		m_ShaderLibrary->Load("assets/shaders/" + shaderName + ".glsl");

		m_DiffuseTexture = VEngine::Texture2D::Create("assets/textures/" + diffuseTex);
		m_SpecularTexture = VEngine::Texture2D::Create("assets/textures/" + specularTex);
	}

	void Material::Draw(glm::mat4& transform)
	{
		auto shader = m_ShaderLibrary->Get(m_ShaderName);
		shader->Bind();

		m_DiffuseTexture->Bind(1);
		m_SpecularTexture->Bind(2);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		//std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		//std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("material.specular", glm::vec3(0.0f, 1.0f, 1.0f));
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("material.shininess", 32.0f);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("light.color", Renderer::s_SceneData->LightColor);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("light.position", Renderer::s_SceneData->LightPos);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", Renderer::s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("material.diffuse", 1); //Get texture form slot = 1
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("material.specular", 2); //Get texture form slot = 2
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("u_AmbientStrength", 1.5f);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("u_CameraPos", Renderer::s_SceneData->CameraPos);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("dirLight.direction", glm::vec3(1.0f, 1.0f, 1.0f));
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("dirLight.color", glm::vec3(0.0f, 1.0f, 1.0f));

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("pointLights[0].color", Renderer::s_SceneData->LightColor);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("pointLights[0].position", Renderer::s_SceneData->LightPos);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("pointLights[0].constant", 1.0f);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("pointLights[0].linear", 1.0f);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("pointLights[0].quadratic", 1.0f);

	}
}