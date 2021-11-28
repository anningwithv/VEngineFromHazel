#include "VEnginePCH.h"
#include <GLFW/glfw3.h>
#include "src/VEngine/Log.h"
#include "src/VEngine/Application.h"

extern VEngine::Application* VEngine::CreateApplication();
//namespace VEngine
//{
	int main(void)
	{
		//GLFWwindow* window;

		///* Initialize the library */
		//if (!glfwInit())
		//	return -1;

		///* Create a windowed mode window and its OpenGL context */
		//window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
		//if (!window)
		//{
		//	glfwTerminate();
		//	return -1;
		//}

		///* Make the window's context current */
		//glfwMakeContextCurrent(window);

		///* Loop until the user closes the window */
		//while (!glfwWindowShouldClose(window))
		//{
		//	/* Render here */
		//	glClear(GL_COLOR_BUFFER_BIT);

		//	/* Swap front and back buffers */
		//	glfwSwapBuffers(window);

		//	/* Poll for and process events */
		//	glfwPollEvents();
		//}

		//glfwTerminate();
		//return 0;

		VEngine::Log::Init();//日志初始化

		VENGINE_CORE_WARN("Engine Init!");//打印日志
		VENGINE_INFO("Hello!");

		auto app = VEngine::CreateApplication();
		app->Run();
		delete app;

		return 0;
	}
//}