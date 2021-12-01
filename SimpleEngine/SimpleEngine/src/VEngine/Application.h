#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Platform/Windows/WinWindow.h"
#include "VEngine/Events/Event.h"
#include "VEngine/Events/ApplicationEvent.h"
#include "VEngine/Layer/LayerStack.h"
#include "Renderer/Shader.h"
#include "ImGui/ImGuiLayer.h"
//#include "Renderer/Buffer.h"
//#include "Renderer/VertexArray.h"
//#include "Renderer/Camera/Camera.h"

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

		ImGuiLayer* m_ImGuiLayer;

		LayerStack m_LayerStack;

		//unsigned int VAO, VBO, EBO;
		//std::shared_ptr<VertexArray> m_VertexArray;
		//std::shared_ptr<Shader> m_Shader;

		//Camera* m_Camera;

	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}