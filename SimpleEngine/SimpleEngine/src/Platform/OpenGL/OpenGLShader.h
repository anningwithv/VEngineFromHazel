#pragma once

#include "VEngine/Renderer/Shader/Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>

namespace VEngine
{
	class OpenGLShader : public Shader
	{
	public:
		//OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filePath);
		virtual ~OpenGLShader() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformInt(const std::string& name, const int value);
		void UploadUniformFloat(const std::string& name, const float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2 value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3 value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4 value);
		void UploadUniformMat3(const std::string& name, const glm::mat3& value);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filePath);
		std::map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
	};
}
