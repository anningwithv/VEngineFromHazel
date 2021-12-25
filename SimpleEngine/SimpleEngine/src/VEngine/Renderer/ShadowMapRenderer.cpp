#include "ShadowMapRenderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "VEngine/Renderer/Renderer.h"
#include "glm/gtc/matrix_transform.hpp"

namespace VEngine
{
	ShadowMapRenderer::ShadowMapRenderer()
	{
		m_ShaderLibrary = std::make_shared<ShaderLibrary>();
		m_ShaderLibrary->Load("assets/shaders/ShadowMapDepth.glsl");
		m_ShaderLibrary->Load("assets/shaders/DebugQuad.glsl");

		// configure depth map FBO
		// -----------------------
		glGenFramebuffers(1, &depthMapFBO);
		// create depth texture
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ShadowMapRenderer::RenderShadowMap(std::vector<Ref<Model>> models)
	{
		auto simpleDepthShader = m_ShaderLibrary->Get("ShadowMapDepth");
		auto debugDepthQuad = m_ShaderLibrary->Get("DebugQuad");
		// 1. render depth of scene to texture (from light's perspective)
		// --------------------------------------------------------------
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(Renderer::s_SceneData->LightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		simpleDepthShader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(simpleDepthShader)->UploadUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, woodTexture);
		RenderScene(models, simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// reset viewport
		glViewport(0, 0, 1280, 720);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render Depth map to quad for visual debugging
		// ---------------------------------------------
		debugDepthQuad->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(debugDepthQuad)->UploadUniformFloat("near_plane", near_plane);
		std::dynamic_pointer_cast<OpenGLShader>(debugDepthQuad)->UploadUniformFloat("far_plane", far_plane);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		RenderQuad();
	}

	// renders the 3D scene
// --------------------
	void ShadowMapRenderer::RenderScene(std::vector<Ref<Model>> models, Ref<Shader> shader)
	{
		auto simpleDepthShader = m_ShaderLibrary->Get("ShadowMapDepth");

		for (int i = 0; i < models.size(); i++)
		{
			glm::mat4 model = models[i]->GetTransform();
			std::dynamic_pointer_cast<OpenGLShader>(simpleDepthShader)->UploadUniformMat4("model", model);
			models[i]->SubmitMesh();
		}
	}

	void ShadowMapRenderer::RenderQuad()
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