#include "VEngine.h"

class ExampleLayer : public VEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (VEngine::Input::IsKeyPressed(VENGINE_KEY_TAB))
			VENGINE_TRACE("Tab key is pressed (poll)!");
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