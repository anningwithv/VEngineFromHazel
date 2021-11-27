#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace VEngine 
{
	class VENGINE_API Application {
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}