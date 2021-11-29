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

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_IsRunning = true;

		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}