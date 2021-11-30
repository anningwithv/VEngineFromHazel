#include "VertexArray.h"
#include "Renderer.h"
#include "VEngine/Core.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace VEngine
{
	VertexArray* VEngine::VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			VENGINE_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}

		VENGINE_CORE_ASSERT(false, "Unknown RendererAPI");

		return nullptr;
	}
}
