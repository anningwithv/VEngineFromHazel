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

		m_Light = std::make_shared<PointLight>(glm::vec3(0.6f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));

		m_Skybox = std::make_shared<SkyboxModel>(m_Light->GetLightPosition(), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), "Skybox", "assets/textures/Box.jpg", "assets/textures/Box_specular.jpg");
		m_CubeModel = std::make_shared<Model>(glm::vec3(1.0f, 0.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), "Standard", "assets/objects/backpack/backpack.obj", "assets/objects/backpack/diffuse.jpg", "assets/objects/backpack/specular.jpg");
		m_LightModel = std::make_shared<CubeModel>(m_Light->GetLightPosition(), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), "Unlit", "assets/textures/Box.jpg", "assets/textures/Box_specular.jpg");
		
		m_CameraController = std::make_shared<CameraController>(Camera::Create(), 1.0f, false);
	}

	void OnUpdate(TimeStep deltaTime) override
	{
		m_CameraController->OnUpdate(deltaTime);

		RendererCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		RendererCommand::Clear();

		Renderer::BeginScene(m_CameraController->GetCamera(), m_Light);

		m_Skybox->Draw(deltaTime);
		m_CubeModel->Draw(deltaTime);
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
	Ref<Model> m_Skybox;

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