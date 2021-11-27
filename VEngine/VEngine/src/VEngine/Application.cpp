#include "VEnginePCH.h"
#include "Application.h"
#include <cstdio>
#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace VEngine 
{
	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() 
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			VENGINE_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			VENGINE_TRACE(e);
		}

		while (true);
	}
}