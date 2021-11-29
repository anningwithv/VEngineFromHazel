#pragma once

#include "GraphicContext.h"

struct GLFWwindow;

namespace VEngine
{
	class OpenGLContext : public GraphicContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}
