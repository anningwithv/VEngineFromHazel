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
		
		float vertices[3 * 9] = 
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f, 1.0f, 0.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 0.0f, 1.0f,
		};

		m_VertexArray.reset(VEngine::VertexArray::Create());

		Ref<VEngine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VEngine::VertexBuffer::Create(vertices, sizeof(vertices)));
		VEngine::BufferLayout layout = 
		{
			{VEngine::ShaderDataType::Float3, "a_Position"},
			{VEngine::ShaderDataType::Float4, "a_Color"},
			{VEngine::ShaderDataType::Float2, "a_TexCoord"},
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//创建索引缓冲对象EBO,它专门储存索引，OpenGL调用这些顶点的索引来决定该绘制哪个顶点
		unsigned int indices[3] = { 0, 1, 2 };
		Ref<VEngine::IndexBuffer> indexBuffer;
		indexBuffer.reset(VEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		/*std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			layout(location = 2) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
			out vec2 v_TexCoord;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = v_Color * texture(u_Texture, v_TexCoord);
			}
		)";*/
		m_ShaderLibrary = std::make_shared<ShaderLibrary>();
		//m_Shader.reset(VEngine::Shader::Create(vertexSrc, fragmentSrc));
		m_ShaderLibrary->Load("assets/shaders/Texture.glsl");

		m_Texture = VEngine::Texture2D::Create("assets/textures/Checkerboard.png");
		m_BlendTexture = VEngine::Texture2D::Create("assets/textures/Logo.png");
	}

	void OnUpdate(TimeStep deltaTime) override
	{
		if (VEngine::Input::IsKeyPressed(VENGINE_KEY_TAB))
			VENGINE_TRACE("Tab key is pressed (poll)!");

		auto shader = m_ShaderLibrary->Get("Texture");

		RendererCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		RendererCommand::Clear();

		Renderer::BeginScene(m_Camera);
		m_Texture->Bind(1);
		Renderer::Submit(shader, m_VertexArray);
		m_BlendTexture->Bind(1);
		Renderer::Submit(shader, m_VertexArray);

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
	//std::shared_ptr<VEngine::Shader> m_Shader;

	VEngine::Camera* m_Camera;
	Ref<Texture2D> m_Texture;
	Ref<Texture2D> m_BlendTexture;
	Ref<ShaderLibrary> m_ShaderLibrary;
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