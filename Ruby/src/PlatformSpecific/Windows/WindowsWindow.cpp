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
namespace Ruby { namespace Renderer { namespace API { void setGladLoadProc(GLADloadproc loadProc); } } }
#endif

namespace Ruby {

	static void glfwErrorCallbackFunc(int error, const char* description)
	{
		RB_ERROR("GLFW Error #%d: %s", error, description);
	}

	static bool s_GLFWinitialized = false;

	WindowsWindow::WindowsWindow(const WindowSpec& spec)
		: m_Name(spec.Name), m_Width(spec.Width), 
		  m_Height(spec.Height), m_VSync(spec.VSync)
	{
		if (!s_GLFWinitialized)
		{
			RB_ASSERT(glfwInit(), "GLFW failed to be initialized.");
			s_GLFWinitialized = true;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Name, nullptr, nullptr);
		
		glfwSetWindowSizeLimits(m_Window, spec.MinWidth, spec.MinHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);
		glfwSetWindowAttrib(m_Window, GLFW_DECORATED, (int)spec.HasTitleBar);

		RB_ASSERT(m_Window, "GLFW window failed to be created.");
		glfwMakeContextCurrent(m_Window);

		glfwSetErrorCallback(glfwErrorCallbackFunc);
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
			{
				WindowCloseEvent e;
				App::getInstance().handleEvent(e);

			});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) 
			{
				WindowResizeEvent e(width, height);
				App::getInstance().handleEvent(e);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
			{
				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent e((KeyCode)key, false);
					App::getInstance().handleEvent(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e((KeyCode)key);
					App::getInstance().handleEvent(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e((KeyCode)key, true);
					App::getInstance().handleEvent(e);
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
				App::getInstance().handleEvent(e);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseReleasedEvent e((MouseCode)button);
				App::getInstance().handleEvent(e);
				break;
			}
			}
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
				
				MouseMoveEvent e((uint16_t)xpos, (uint16_t)ypos);
				App::getInstance().handleEvent(e);
				
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
				
				MouseScrollEvent e((float)xoffset, (float)yoffset);
				App::getInstance().handleEvent(e);

			});

		glfwSwapInterval((int)m_VSync);

	#ifdef RB_USE_OPENGL
		Renderer::API::setGladLoadProc((GLADloadproc)glfwGetProcAddress);
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

	bool WindowsWindow::isKeyDown(KeyCode code) const
	{
		return glfwGetKey(m_Window, code) == GLFW_PRESS;
	}

	bool WindowsWindow::isMouseButtonDown(MouseCode code) const
	{
		return glfwGetMouseButton(m_Window, code) == GLFW_PRESS;
	}

}