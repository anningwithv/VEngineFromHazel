#include "SSAORenderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "VEngine/Renderer/Renderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include <random>

namespace VEngine
{
	SSAORenderer::SSAORenderer(unsigned int gPositionBuffer, unsigned int gNormalBuffer)
	{
		this->gPositionBuffer = gPositionBuffer;
		this->gNormalBuffer = gNormalBuffer;

		m_ShaderLibrary = std::make_shared<ShaderLibrary>();
		m_ShaderLibrary->Load("assets/shaders/SSAO.glsl");
		m_ShaderLibrary->Load("assets/shaders/SSAOBlur.glsl");

		auto shaderSSAO = m_ShaderLibrary->Get("SSAO");
		shaderSSAO->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shaderSSAO)->UploadUniformInt("gPosition", 0);
		std::dynamic_pointer_cast<OpenGLShader>(shaderSSAO)->UploadUniformInt("gNormal", 1);
		std::dynamic_pointer_cast<OpenGLShader>(shaderSSAO)->UploadUniformInt("texNoise", 2);

		auto shaderSSAOBlur = m_ShaderLibrary->Get("SSAOBlur");
		shaderSSAOBlur->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shaderSSAOBlur)->UploadUniformInt("ssaoInput", 0);
	
	
		// generate sample kernel
	// ----------------------
		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;
		//std::vector<glm::vec3> ssaoKernel;
		for (unsigned int i = 0; i < 64; ++i)
		{
			glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			float scale = float(i) / 64.0;

			// scale samples s.t. they're more aligned to center of kernel
			scale = Lerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}

		// create framebuffer to hold SSAO processing stage 
	    // -----------------------------------------------------
		//unsigned int ssaoFBO, ssaoBlurFBO;
		glGenFramebuffers(1, &ssaoFBO);  glGenFramebuffers(1, &ssaoBlurFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
		//unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
		// SSAO color buffer
		glGenTextures(1, &ssaoColorBuffer);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Renderer::SCR_WIDTH, Renderer::SCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			VENGINE_ERROR("SSAO Framebuffer not complete");
		// and blur stage
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		glGenTextures(1, &ssaoColorBufferBlur);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Renderer::SCR_WIDTH, Renderer::SCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			VENGINE_ERROR("SSAO Blur Framebuffer not complete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// generate noise texture
		// ----------------------
		//std::vector<glm::vec3> ssaoNoise;
		for (unsigned int i = 0; i < 16; i++)
		{
			glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
			ssaoNoise.push_back(noise);
		}
		glGenTextures(1, &gNoiseTex);
		glBindTexture(GL_TEXTURE_2D, gNoiseTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}

	void SSAORenderer::Render()
	{
		auto shaderSSAO = m_ShaderLibrary->Get("SSAO");
		auto shaderSSAOBlur = m_ShaderLibrary->Get("SSAOBlur");

		// 1. geometry pass: render scene's geometry/color data into gbuffer
		// -----------------------------------------------------------------

		// 2. generate SSAO texture
		// ------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderSSAO->Bind();

		// Send kernel + rotation 
		for (unsigned int i = 0; i < 64; ++i)
		{
			std::dynamic_pointer_cast<OpenGLShader>(shaderSSAO)->UploadUniformFloat3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);

		}
		std::dynamic_pointer_cast<OpenGLShader>(shaderSSAO)->UploadUniformMat4("projection", Renderer::s_SceneData->ProjectionMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPositionBuffer);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormalBuffer);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gNoiseTex);
		RenderQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		// 3. blur SSAO texture to remove noise
		// ------------------------------------
		//glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		//glClear(GL_COLOR_BUFFER_BIT);
		//shaderSSAOBlur->Bind();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		//RenderQuad();
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void SSAORenderer::RenderQuad()
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

	float SSAORenderer::Lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}
}