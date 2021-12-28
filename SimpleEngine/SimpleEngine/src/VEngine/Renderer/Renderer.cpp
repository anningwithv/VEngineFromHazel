#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace VEngine
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;
	Camera* Renderer::s_Camera;
	ShadowMapRenderer* Renderer::s_ShadowMapRenderer;
	GBufferRenderer* Renderer::s_GBufferRenderer;

	unsigned int Renderer::SCR_WIDTH = 1280;
	unsigned int Renderer::SCR_HEIGHT = 760;

	void Renderer::Init()
	{
		RendererCommand::Init();
	}

	void Renderer::BeginScene(Camera* camera, const Ref<Light>& light, std::vector<Ref<Model>> models)
	{
		s_Camera = camera;

		s_SceneData->CameraPos = s_Camera->GetPosition();
		s_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
		s_SceneData->ViewMatrix = camera->GetViewMatrix();
		s_SceneData->ProjectionMatrix = camera->GetProjectionMatrix();

		s_SceneData->LightPos = light->GetLightPosition();
		s_SceneData->LightColor = light->GetLightColor();

		if (s_ShadowMapRenderer == nullptr)
		{
			s_ShadowMapRenderer = new ShadowMapRenderer();
		}
		s_ShadowMapRenderer->RenderShadowMap(models);

		if (s_GBufferRenderer == nullptr)
		{
			s_GBufferRenderer = new GBufferRenderer();
		}
		s_GBufferRenderer->Render(models);
	}

	void Renderer::EndScene(Camera*  camera)
	{
	}

	void Renderer::Submit(/*const std::shared_ptr<Shader>& shader,*/ const std::shared_ptr<VertexArray>& vertexArray,
		const glm::mat4& transform)
	{
		s_SceneData->CameraPos = s_Camera->GetPosition();
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