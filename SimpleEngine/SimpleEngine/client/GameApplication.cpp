#include "VEngine.h"

using namespace VEngine;

class ExampleLayer : public VEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		GameMode::SetGameMode(GameMode::Mode::D2);

		m_Camera.reset(VEngine::Camera::Create());
		
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
	
		m_ShaderLibrary = std::make_shared<ShaderLibrary>();
		m_ShaderLibrary->Load("assets/shaders/Texture.glsl");

		m_Texture = VEngine::Texture2D::Create("assets/textures/Checkerboard.png");
		m_BlendTexture = VEngine::Texture2D::Create("assets/textures/Logo.png");

		m_CameraController = std::make_shared<OrthographicCameraController>(1.0f, false);
	}

	void OnUpdate(TimeStep deltaTime) override
	{
		m_CameraController->OnUpdate(deltaTime);

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
		m_CameraController->OnEvent(event);
	}

private:
	Ref<VEngine::VertexArray> m_VertexArray;

	Ref<Camera> m_Camera;
	Ref<Texture2D> m_Texture;
	Ref<Texture2D> m_BlendTexture;
	Ref<ShaderLibrary> m_ShaderLibrary;
	Ref<OrthographicCameraController> m_CameraController;
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