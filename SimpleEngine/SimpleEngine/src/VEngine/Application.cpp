#include "VEnginePCH.h"
#include "Application.h"
#include "Log.h"
#include "Input.h"

namespace VEngine 
{
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case VEngine::ShaderDataType::Float:
		case VEngine::ShaderDataType::Float2:
		case VEngine::ShaderDataType::Float3:
		case VEngine::ShaderDataType::Float4:
		case VEngine::ShaderDataType::Mat3:
		case VEngine::ShaderDataType::Mat4:
			return GL_FLOAT;
		case VEngine::ShaderDataType::Int:
		case VEngine::ShaderDataType::Int2:
		case VEngine::ShaderDataType::Int3:
		case VEngine::ShaderDataType::Int4:
			return GL_INT;
		case VEngine::ShaderDataType::Bool:
			return GL_BOOL;
		default:
			break;
		}

		VENGINE_CORE_ASSERT(false, "Unknown shader data type!");

		return 0;
	}

	Application::Application() 
	{
		s_Instance = this;

		m_Window = std::unique_ptr<WinWindow>( WinWindow::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		//m_ImGuiLayer = new ImGuiLayer();
		//PushOverlay(m_ImGuiLayer);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		};

		//Generate VAO
		glGenVertexArrays(1, &VAO);
		//Bind VAO
		glBindVertexArray(VAO);

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
		};

		m_VertexBuffer->SetLayout(layout);

		unsigned int index = 0;
		for (const auto& element : m_VertexBuffer->GetLayout())
		{
			//glVertexAttribPointer��������OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ�
			//��һ������ָ������Ҫ���õĶ������ԡ����ǵ������ڶ�����ɫ����ʹ��layout(location = 0)
			//������position�������Ե�λ��ֵ(Location)�������԰Ѷ������Ե�λ��ֵ����Ϊ0��
			//��Ϊ����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��
			//�ڶ�������ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
			//����������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)��
			//���ĸ��������������Ƿ�ϣ�����ݱ���׼��(Normalize)
			//�����������������(Stride)�������������������Ķ���������֮��ļ����
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, m_VertexBuffer->GetLayout().GetStride(), (const void*)element.Offset);
			//���ö�������,Ĭ���ǽ�ֹ��
			glEnableVertexAttribArray(index);

			index++;
		}

		//���������������EBO,��ר�Ŵ���������OpenGL������Щ����������������û����ĸ�����
		unsigned int indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(unsigned int)));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = v_Color;
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
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
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