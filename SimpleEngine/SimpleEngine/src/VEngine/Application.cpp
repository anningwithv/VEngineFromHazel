#include "VEnginePCH.h"
#include "Application.h"
#include "Log.h"
#include "Input.h"
//#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "VEngine/Renderer/Renderer.h"
//#include "GameMode.h"

namespace VEngine 
{
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{
		s_Instance = this;

		m_Window = std::unique_ptr<WinWindow>( WinWindow::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();
	}

	Application::~Application() {

	}

	void Application::Run() 
	{
		float time = (float)glfwGetTime();
		TimeStep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		while (m_IsRunning)
		{
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//VENGINE_CORE_TRACE("{0}", e);
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		//layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		//layer->OnAttach();
	}

	//Application * CreateApplication()
	//{
	//	return new Application();
	//}
}