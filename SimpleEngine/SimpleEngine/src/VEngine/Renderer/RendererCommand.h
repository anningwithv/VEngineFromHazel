#pragma once

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace VEngine
{
	class RendererCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetClearColor(const glm::vec4& color) 
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawArray(const std::shared_ptr<VertexArray>& vertexArray, int count)
		{
			s_RendererAPI->DrawArray(vertexArray, count);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}
