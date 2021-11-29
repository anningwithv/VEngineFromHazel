#include "VEnginePCH.h"
#include "Application.h"
//#include <cstdio>
#include "Log.h"
#include "Input.h"

namespace VEngine 
{
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{
		s_Instance = this;

		m_Window = std::unique_ptr<WinWindow>( WinWindow::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		//m_ImGuiLayer = new ImGuiLayer();
		//PushOverlay(m_ImGuiLayer);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		//Generate VAO
		glGenVertexArrays(1, &VAO);
		//Bind VAO
		glBindVertexArray(VAO);

		//Generate VBO
		glGenBuffers(1, &VBO);
		//Bind VBO to GL_ARRAY_BUFFER
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData is used to set custom data to current ArrayBuffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		//glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
		//第一个参数指定我们要配置的顶点属性。还记得我们在顶点着色器中使用layout(location = 0)
		//定义了position顶点属性的位置值(Location)吗？它可以把顶点属性的位置值设置为0。
		//因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
		//第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
		//第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
		//第四个参数定义我们是否希望数据被标准化(Normalize)
		//第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		//启用顶点属性,默认是禁止的
		glEnableVertexAttribArray(0);

		//创建索引缓冲对象EBO,它专门储存索引，OpenGL调用这些顶点的索引来决定该绘制哪个顶点
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	Application::~Application() {

	}

	void Application::Run() 
	{
		while (m_IsRunning)
		{
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 3);
			//glBindVertexArray(0);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

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
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	//Application * CreateApplication()
	//{
	//	return new Application();
	//}
}