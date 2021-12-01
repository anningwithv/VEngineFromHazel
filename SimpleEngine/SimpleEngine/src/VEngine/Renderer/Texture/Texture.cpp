#include "Texture.h"
#include "VEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace VEngine
{
	Ref<Texture2D> VEngine::Texture2D::Create(const std::string & path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture>(path);
		default:
			break;
		}

		VENGINE_CORE_ASSERT(false, "Unkown RendererAPI");

		return nullptr;
	}
}