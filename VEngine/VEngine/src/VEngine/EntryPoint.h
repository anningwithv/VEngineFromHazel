#pragma once

#ifdef VENGINE_PLATFORM_WINDOWS

extern VEngine::Application* VEngine::CreateApplication();

int main(int argc, char** argv) 
{
	VEngine::Log::Init();//日志初始化

	VENGINE_CORE_WARN("Engine Init!");//打印日志
	VENGINE_INFO("Hello!");

	auto app = VEngine::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif
