#include "VEngine.h"

using namespace VEngine;

class ExampleLayer : public VEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		GameMode::SetGameMode(GameMode::Mode::D2);

		m_Camera = VEngine::Camera::Create();
		
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		};

		m_VertexArray.reset(VEngine::VertexArray::Create());

		std::shared_ptr<VEngine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VEngine::VertexBuffer::Create(vertices, sizeof(vertices)));
		VEngine::BufferLayout layout = {
			{VEngine::ShaderDataType::Float3, "a_Position"},
			{VEngine::ShaderDataType::Float4, "a_Color"},
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//创建索引缓冲对象EBO,它专门储存索引，OpenGL调用这些顶点的索引来决定该绘制哪个顶点
		unsigned int indices[3] = { 0, 1, 2 };
		std::shared_ptr<VEngine::IndexBuffer> indexBuffer;
		indexBuffer.reset(VEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
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

		m_Shader.reset(new VEngine::Shader(vertexSrc, fragmentSrc));
	}

	void OnUpdate(TimeStep deltaTime) override
	{
		if (VEngine::Input::IsKeyPressed(VENGINE_KEY_TAB))
			VENGINE_TRACE("Tab key is pressed (poll)!");

		RendererCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		RendererCommand::Clear();

		Renderer::BeginScene(m_Camera);
		Renderer::Submit(m_Shader, m_VertexArray);
		Renderer::EndScene(m_Camera);
	}

	void OnEvent(VEngine::Event& event) override
	{
		if (event.GetEventType() == VEngine::EventType::KeyPressed)
		{
			VEngine::KeyPressedEvent& e = (VEngine::KeyPressedEvent&)event;
			if (e.GetKeyCode() == VENGINE_KEY_TAB)
				VENGINE_TRACE("Tab key is pressed (event)!");
			VENGINE_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

private:
	std::shared_ptr<VEngine::VertexArray> m_VertexArray;
	std::shared_ptr<VEngine::Shader> m_Shader;

	VEngine::Camera* m_Camera;
};

class GameApplication : public VEngine::Application
{
public:
	GameApplication()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new VEngine::ImGuiLayer());
	}

	~GameApplication()
	{
	}

};

VEngine::Application* VEngine::CreateApplication()
{
	return new GameApplication();
}