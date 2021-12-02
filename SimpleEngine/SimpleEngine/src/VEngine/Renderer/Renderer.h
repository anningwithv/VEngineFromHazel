#pragma once

#include "RendererCommand.h"
#include "Camera/Camera.h"
#include "Shader/Shader.h"

namespace VEngine
{
	class Renderer
	{
	public:
		static void Init();
		static void BeginScene(Camera* camera);
		static void EndScene(Camera* camera);
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* s_SceneData;
	};
}
