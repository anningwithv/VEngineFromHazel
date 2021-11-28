#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Platform/Windows/WinWindow.h"

namespace VEngine 
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	private:
		WinWindow* m_Window;
		bool m_IsRunning = true;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}