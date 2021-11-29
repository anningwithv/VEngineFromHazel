#include "VEngine.h"

//namespace Client
//{
	class GameApplication : public VEngine::Application
	{
	public:
		GameApplication()
		{
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
//}