#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace VEngine
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RendererCommand::Init();
	}

	void Renderer::BeginScene(Camera* camera, const Ref<Light>& light)
	{
		s_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();

		s_SceneData->LightPos = light->GetLightPosition();
	}

	void Renderer::EndScene(Camera*  camera)
	{
	}

	void Renderer::Submit(/*const std::shared_ptr<Shader>& shader,*/ const std::shared_ptr<VertexArray>& vertexArray,
		const glm::mat4& transform)
	{
		//shader->Bind();

		//std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		//std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		//std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("u_Texture", 1); //Get texture form slot = 1
		//std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat4("u_LightColor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		//std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat("u_AmbientStrength", 1.5f);
		//std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("u_LightPos", glm::vec3(1.0f, 1.0f, 1.0f));
		vertexArray->Bind();

		RendererCommand::DrawIndexed(vertexArray);
		//RendererCommand::DrawArray(vertexArray, 36);
	}
}