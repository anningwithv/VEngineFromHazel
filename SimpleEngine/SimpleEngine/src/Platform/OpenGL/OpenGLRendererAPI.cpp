#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

namespace VEngine
{
	void OpenGLRendererAPI::Init()
	{
		//Enable blend
		//glEnable(GL_BLEND);
		//FinalColor = ScrColor*SrcAlpha + DstColor*(1-SrcAlpha)
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//Other options:GL_ZERO GL_ONE GL_DST_ALPHA GL_ONE_MINUS_SRC_ALPHA GL_ONE_MINUS_DST_ALPHA etc...
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}

	void VEngine::OpenGLRendererAPI::SetClearColor(const glm::vec4 & color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void VEngine::OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void VEngine::OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
	}

	void VEngine::OpenGLRendererAPI::DrawArray(const std::shared_ptr<VertexArray>& vertexArray, int count)
	{
		glDrawArrays(GL_TRIANGLES, 0, count);
	}
}