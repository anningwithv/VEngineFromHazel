#pragma once

#ifdef VENGINE_PLATFORM_WINDOWS

extern VEngine::Application* VEngine::CreateApplication();

int main(int argc, char** argv) 
{
	VEngine::Log::Init();//��־��ʼ��

	VENGINE_CORE_WARN("Engine Init!");//��ӡ��־
	VENGINE_INFO("Hello!");

	auto app = VEngine::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif
