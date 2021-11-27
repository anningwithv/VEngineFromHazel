#pragma once

#include "Core.h"

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