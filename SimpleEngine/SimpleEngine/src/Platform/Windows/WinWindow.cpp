#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include  "glad/glad.h"
#include "VEnginePCH.h"
#include "WinWindow.h"
#include "VEngine/Log.h"
#include "VEngine/Events/ApplicationEvent.h"
#include "VEngine/Events/MouseEvent.h"
#include "VEngine/Events/KeyEvent.h"
#include "VEngine/Renderer/OpenGLContext.h"

namespace VEngine
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* desc)
	{
		VENGINE_CORE_ERROR("GLFW Error ({0}): {1}", error, desc);
	}

	WinWindow* WinWindow::Create(const WindowProps& props)
	{
		return new WinWindow(props);
	}

	WinWindow::WinWindow(const WindowProps& props)
	{
		Init(props);
	}

	WinWindow::~WinWindow()
	{
		Shutdown();
	}

	void WinWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		VENGINE_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			VENGINE_CORE_ASSERT(success, "Could not intialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		
		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		//Set GLFW Callbacks
		SetGLFWCallbacks();

	
	}

	void WinWindow::SetGLFWCallbacks()
	{
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keyCode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(keyCode);
			data.EventCallback(event);
		});
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent pressEvent(key, 0);
				data.EventCallback(pressEvent);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent releaseEvent(key);
				data.EventCallback(releaseEvent);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyReleasedEvent repeatEvent(key);
				data.EventCallback(repeatEvent);
				break;
			}
			}
		});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent pressEvent(button);
				data.EventCallback(pressEvent);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent releaseEvent(button);
				data.EventCallback(releaseEvent);
				break;
			}
			}
		});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}

	void WinWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WinWindow::OnUpdate()
	{
		glfwPollEvents();
		
		m_Context->SwapBuffers();
	}

	void WinWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WinWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}
