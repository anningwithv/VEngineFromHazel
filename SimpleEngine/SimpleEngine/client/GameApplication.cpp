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

		m_Light = std::make_shared<PointLight>(glm::vec3(0.6f, 0.5f, 0.5f), glm::vec3(-2.0f, 1.0f, 1.0f));

		auto skyMat = std::make_shared<Material>("Skybox", "assets/textures/Box.jpg", "assets/textures/Box_specular.jpg");
		m_Skybox = std::make_shared<SkyboxModel>(m_Light->GetLightPosition(), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), skyMat);
		
		//auto bagModel = std::make_shared<Model>(glm::vec3(1.0f, 0.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), "Standard", "assets/objects/backpack/backpack.obj", "assets/objects/backpack/diffuse.jpg", "assets/objects/backpack/specular.jpg");
		//m_Models.push_back(bagModel);
		auto mat1 = std::make_shared<Material>("Standard", "assets/textures/Box.jpg", "assets/textures/Box_specular.jpg");
		auto bigCube = std::make_shared<CubeModel>(glm::vec3(1.0f, -2.0f, -5.0f), glm::vec3(30.0f, 0.5f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), mat1);
		m_Models.push_back(bigCube);
		auto mat2 = std::make_shared<PBRMaterial>("PBR", "assets/textures/Box.jpg", "assets/textures/Box_specular.jpg");
		auto smallCube = std::make_shared<CubeModel>(glm::vec3(0.0f, -1.0f, -4.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), mat2);
		m_Models.push_back(smallCube);
		//auto shpere = std::make_shared<SphereModel>(glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), "Standard", "assets/textures/Box.jpg", "assets/textures/Box_specular.jpg");
		//m_Models.push_back(shpere);
		//auto lightModel = std::make_shared<CubeModel>(m_Light->GetLightPosition(), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), "Unlit", "assets/textures/Box.jpg", "assets/textures/Box_specular.jpg");
		//m_Models.push_back(lightModel);

		m_CameraController = std::make_shared<CameraController>(Camera::Create(), 1.0f, false);
	}

	void OnUpdate(TimeStep deltaTime) override
	{
		m_CameraController->OnUpdate(deltaTime);

		RendererCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		RendererCommand::Clear();

		Renderer::BeginScene(m_CameraController->GetCamera(), m_Light, m_Models);

		m_Skybox->Draw(deltaTime);


		for (int i = 0; i < m_Models.size(); i++)
		{
			m_Models[i]->Draw(deltaTime);
		}

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
	std::vector<Ref<Model>> m_Models;

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