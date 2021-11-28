#include "VEnginePCH.h"
#include "Application.h"
#include <cstdio>
#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace VEngine 
{
	Application::Application() 
	{
		m_Window = WinWindow::Create();
	}

	Application::~Application() {

	}

	void Application::Run() 
	{
		while (m_IsRunning)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	Application * CreateApplication()
	{
		return new Application();
	}
}