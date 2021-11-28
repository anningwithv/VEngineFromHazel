#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Platform/Windows/WinWindow.h"
#include "VEngine/Events/Event.h"
#include "VEngine/Events/ApplicationEvent.h"

namespace VEngine 
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<WinWindow> m_Window;
		bool m_IsRunning = true;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}