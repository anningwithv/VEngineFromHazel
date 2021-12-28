#include "GBufferRenderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "VEngine/Renderer/Renderer.h"
#include "glm/gtc/matrix_transform.hpp"

namespace VEngine
{
	GBufferRenderer::GBufferRenderer()
	{
		m_ShaderLibrary = std::make_shared<ShaderLibrary>();
		m_ShaderLibrary->Load("assets/shaders/DeferredShading.glsl");
		m_ShaderLibrary->Load("assets/shaders/GBuffer.glsl");
		// configure g-buffer framebuffer
		// ------------------------------
		glGenFramebuffers(1, &gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		// position color buffer
		glGenTextures(1, &gPosition);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Renderer::SCR_WIDTH, Renderer::SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
		// normal color buffer
		glGenTextures(1, &gNormal);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Renderer::SCR_WIDTH, Renderer::SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
		// color + specular color buffer
		glGenTextures(1, &gAlbedoSpec);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Renderer::SCR_WIDTH, Renderer::SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);
		// create and attach depth buffer (renderbuffer)
		unsigned int rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Renderer::SCR_WIDTH, Renderer::SCR_HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			VENGINE_ERROR("Framebuffer not complete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		m_DiffuseTexture = VEngine::Texture2D::Create("assets/textures/Box.jpg");
		m_SpecularTexture = VEngine::Texture2D::Create("assets/textures/Box_specular.jpg");


		// shader configuration
		// --------------------
		auto deferedRenderShader = m_ShaderLibrary->Get("DeferredShading");
		deferedRenderShader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(deferedRenderShader)->UploadUniformInt("gPosition", 0);
		std::dynamic_pointer_cast<OpenGLShader>(deferedRenderShader)->UploadUniformInt("gNormal", 1);
		std::dynamic_pointer_cast<OpenGLShader>(deferedRenderShader)->UploadUniformInt("gAlbedoSpec", 2);

	}

	void GBufferRenderer::Render(std::vector<Ref<Model>> models)
	{
		auto gBufferShader = m_ShaderLibrary->Get("GBuffer");
		auto deferedRenderShader = m_ShaderLibrary->Get("DeferredShading");

		// 1. geometry pass: render scene's geometry/color data into gbuffer
		// -----------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = Renderer::s_SceneData->ProjectionMatrix;
		glm::mat4 view = Renderer::s_SceneData->ViewMatrix;
		gBufferShader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(gBufferShader)->UploadUniformMat4("projection", projection);
		std::dynamic_pointer_cast<OpenGLShader>(gBufferShader)->UploadUniformMat4("view", view);

		RenderScene(models, gBufferShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
		// -----------------------------------------------------------------------------------------------------------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		deferedRenderShader->Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		// send light relevant uniforms
		for (unsigned int i = 0; i < 1; i++)
		{
			std::dynamic_pointer_cast<OpenGLShader>(deferedRenderShader)->UploadUniformFloat3("lights[" + std::to_string(i) + "].Position", Renderer::s_SceneData->LightPos);
			std::dynamic_pointer_cast<OpenGLShader>(deferedRenderShader)->UploadUniformFloat3("lights[" + std::to_string(i) + "].Color", Renderer::s_SceneData->LightColor);

			// update attenuation parameters and calculate radius
			const float linear = 0.7;
			const float quadratic = 1.8;
			std::dynamic_pointer_cast<OpenGLShader>(deferedRenderShader)->UploadUniformFloat("lights[" + std::to_string(i) + "].Linear", linear);
			std::dynamic_pointer_cast<OpenGLShader>(deferedRenderShader)->UploadUniformFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
		}
		std::dynamic_pointer_cast<OpenGLShader>(deferedRenderShader)->UploadUniformFloat3("viewPos", Renderer::s_SceneData->CameraPos);

		// finally render quad
		RenderQuad();

	}

	void GBufferRenderer::RenderScene(std::vector<Ref<Model>> models, Ref<Shader> shader)
	{
		m_DiffuseTexture->Bind(3);
		m_SpecularTexture->Bind(4);

		for (int i = 0; i < models.size(); i++)
		{
			glm::mat4 model = models[i]->GetTransform();
			std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("model", model);
			std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("texture_diffuse1", 3); //Get texture form slot = 1
			std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("texture_specular1", 4); //Get texture form slot = 2
			models[i]->SubmitMesh();
		}
	}

	void GBufferRenderer::RenderQuad()
	{
		unsigned int quadVAO = 0;
		unsigned int quadVBO;

		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
}