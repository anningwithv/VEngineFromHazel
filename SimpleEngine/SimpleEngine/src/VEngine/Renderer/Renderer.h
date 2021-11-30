#pragma once

#include "RendererCommand.h"
#include "Camera/Camera.h"
#include "Shader.h"

namespace VEngine
{
	class Renderer
	{
	public:
		static void BeginScene(Camera* camera);
		static void EndScene(Camera* camera);
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* s_SceneData;
	};
}
