#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace VEngine
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RendererCommand::Init();
	}

	void Renderer::BeginScene(Ref<Camera> camera)
	{
		s_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene(Ref<Camera> camera)
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
		const glm::mat4& transform)
	{
		shader->Bind();

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("u_Texture", 1); //Get texture form slot = 1
		
		vertexArray->Bind();

		RendererCommand::DrawIndexed(vertexArray);
	}
}