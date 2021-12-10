#include "VEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace VEngine;

class ExampleLayer : public VEngine::Layer
{
public:
	ExampleLayer(): Layer("Example")
	{
		GameMode::SetGameMode(GameMode::Mode::D3);

		m_Light = std::make_shared<PointLight>(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		m_CubeModel = std::make_shared<Model>(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 45.0f, 0.0f), "Standard");
		m_LightModel = std::make_shared<Model>(m_Light->GetLightPosition(), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), "Standard");

		m_CameraController = std::make_shared<CameraController>(Camera::Create(), 1.0f, false);
	}

	void OnUpdate(TimeStep deltaTime) override
	{
		m_CameraController->OnUpdate(deltaTime);

		RendererCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		RendererCommand::Clear();

		Renderer::BeginScene(m_CameraController->GetCamera(), m_Light);

		//m_CubeModel->Draw(deltaTime);
		m_LightModel->Draw(deltaTime);

		Renderer::EndScene(m_CameraController->GetCamera());
	}

	void OnEvent(VEngine::Event& event) override
	{
		m_CameraController->OnEvent(event);
	}

private:
	Ref<VEngine::VertexArray> m_VertexArray;

	Ref<Texture2D> m_Texture;
	Ref<Texture2D> m_BlendTexture;
	Ref<ShaderLibrary> m_ShaderLibrary;
	//Ref<Mesh> m_Mesh;
	Ref<Model> m_CubeModel;
	Ref<Model> m_LightModel;

	Ref<Light> m_Light;

	Ref<CameraController> m_CameraController;
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