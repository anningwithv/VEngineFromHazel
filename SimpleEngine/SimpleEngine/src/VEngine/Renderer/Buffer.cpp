#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace VEngine
{
	VertexBuffer * VEngine::VertexBuffer::Create(float * vertices, unsigned int size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}
		return nullptr;
	}

	IndexBuffer* VEngine::IndexBuffer::Create(unsigned int* indices, unsigned int count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
		}
		return nullptr;
	}
}