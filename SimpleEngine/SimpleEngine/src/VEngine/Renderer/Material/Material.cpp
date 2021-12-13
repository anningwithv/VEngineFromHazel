#include "Material.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "VEngine/Renderer/Renderer.h"

namespace VEngine
{
	Material::Material(const std::string& shaderName, const std::string& textureName)
	{
		m_ShaderName = shaderName;

		m_ShaderLibrary = std::make_shared<ShaderLibrary>();
		m_ShaderLibrary->Load("assets/shaders/" + shaderName + ".glsl");

		m_DiffuseTexture = VEngine::Texture2D::Create("assets/textures/" + textureName);
		m_SpecularTexture = VEngine::Texture2D::Create("assets/textures/Box_specular.jpg");
	}

	void Material::Draw(glm::mat4& transform)
	{
		auto shader = m_ShaderLibrary->Get(m_ShaderName);
		shader->Bind();

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("material.specular", glm::vec3(0.0f, 1.0f, 1.0f));
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("material.shininess", 32.0f);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("light.color", Renderer::s_SceneData->LightColor);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("light.position", Renderer::s_SceneData->LightPos);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", Renderer::s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("u_Texture", 1); //Get texture form slot = 1
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("u_AmbientStrength", 1.5f);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("u_CameraPos", Renderer::s_SceneData->CameraPos);

		m_DiffuseTexture->Bind(1);

	}
}