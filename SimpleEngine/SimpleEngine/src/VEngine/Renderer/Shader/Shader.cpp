#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "VEngine/Renderer/Renderer.h"

namespace VEngine
{
	Shader * VEngine::Shader::Create(const std::string & vertexSrc, const std::string & fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}
		return nullptr;
	}
}
