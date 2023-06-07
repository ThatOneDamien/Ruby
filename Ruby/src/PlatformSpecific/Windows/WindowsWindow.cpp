#include "ruby_pch.h"

#include "PlatformSpecific/Windows/WindowsWindow.h"

#include "Ruby/Main/Core.h"
#include "Ruby/Main/App.h"

#include "Ruby/Event/AppEvent.h"
#include "Ruby/Event/KeyEvent.h"
#include "Ruby/Event/MouseEvent.h"


#include <GLFW/glfw3.h>

// OpenGL specific code that allows Glad to be initialized without directly including glad.h in this file.
#ifdef RB_USE_OPENGL
//FROM glad.h
typedef void*(*GLADloadproc)(const char*);
namespace Ruby { namespace Renderer { void setGladLoadProc(GLADloadproc loadProc); } }
#endif

namespace Ruby {

	static void glfwErrorCallbackFunc(int error, const char* description)
	{
		RB_ERROR("GLFW Error #%d: %s", error, description);
	}

	static bool s_GLFWinitialized = false;

	WindowsWindow::WindowsWindow(const char* name, uint16_t width, uint16_t height, bool vSync)
		: m_Name(name), m_Width(width), m_Height(height), m_VSync(vSync)
	{
		if (!s_GLFWinitialized)
		{
			RB_ASSERT(glfwInit(), "GLFW failed to be initialized.");
			s_GLFWinitialized = true;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(width, height, name, nullptr, nullptr);
		RB_ASSERT(m_Window, "GLFW window failed to be created.");
		glfwMakeContextCurrent(m_Window);

		glfwSetErrorCallback(glfwErrorCallbackFunc);

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
			{
				WindowCloseEvent e;
				App::getInstance().onEvent(e);

			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
			{
				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent e((KeyCode)key, false);
					App::getInstance().onEvent(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e((KeyCode)key);
					App::getInstance().onEvent(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e((KeyCode)key, true);
					App::getInstance().onEvent(e);
					break;
				}
					
				}

			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {

			switch (action)
			{
			case GLFW_PRESS:
			{
				MousePressedEvent e((MouseCode)button);
				App::getInstance().onEvent(e);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseReleasedEvent e((MouseCode)button);
				App::getInstance().onEvent(e);
				break;
			}
			}
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
				
				MouseMoveEvent e((uint16_t)xpos, (uint16_t)ypos);
				App::getInstance().onEvent(e);
				
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
				
				MouseScrollEvent e((float)xoffset, (float)yoffset);
				App::getInstance().onEvent(e);

			});

		glfwSwapInterval((int)vSync);

	#ifdef RB_USE_OPENGL
		Renderer::setGladLoadProc((GLADloadproc)glfwGetProcAddress);
	#endif
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		glfwSwapInterval((int)enabled);
		m_VSync = enabled;
	}

	void WindowsWindow::update() const
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

}