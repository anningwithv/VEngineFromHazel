#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Platform/Windows/WinWindow.h"
#include "VEngine/Events/Event.h"
#include "VEngine/Events/ApplicationEvent.h"
#include "VEngine/Layer/LayerStack.h"

namespace VEngine 
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<WinWindow> m_Window;
		bool m_IsRunning = true;

		LayerStack m_LayerStack;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}