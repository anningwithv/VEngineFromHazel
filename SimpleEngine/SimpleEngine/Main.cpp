#include "VEnginePCH.h"
#include <GLFW/glfw3.h>
#include "src/VEngine/Log.h"
#include "src/VEngine/Application.h"

extern VEngine::Application* VEngine::CreateApplication();

int main(void)
{
	VEngine::Log::Init();//日志初始化

	VENGINE_CORE_WARN("Engine Init!");//打印日志
	VENGINE_INFO("Hello!");

	auto app = VEngine::CreateApplication();
	app->Run();
	delete app;

	return 0;
}
