#include "ruby_pch.h"

#include "App.h"
#include "Core.h"
#include "Window.h"

#include <GLFW/glfw3.h>

#ifdef RB_PLAT_WIND
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

namespace Ruby
{
#ifdef RB_DEBUG
    static void glfwErrorCallbackFunc(int error, const char* description)
    {
        RB_ERROR("GLFW Error #%d: %s", error, description);
    }
#endif

    static bool   s_GLFWinitialized = false;
    static size_t s_WindowCount = 0;

    Window::Window(const WindowSpec& spec, API desiredAPI)
        : m_Title(spec.Name), m_Width(spec.Width),
        m_Height(spec.Height), m_VSync(spec.VSync)
    {
        if (!s_GLFWinitialized)
        {
            RB_ENSURE_CRITICAL(glfwInit(), "GLFW failed to be initialized.");
            s_GLFWinitialized = true;
            RB_INFO("GLFW initialized successfully!");
        }
        ++s_WindowCount;

        switch(desiredAPI)
        {
        case API::OpenGL:
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            break;
        case API::Vulkan:
            // TODO: FILL IN
            break;
        default:
            RB_ERROR_DEBUG("Unrecognized Graphics API.");
        }

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

        glfwSetWindowSizeLimits(m_Window, spec.MinWidth, spec.MinHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);
        glfwSetWindowAttrib(m_Window, GLFW_DECORATED, (int)spec.HasTitleBar);

        RB_ENSURE_CRITICAL(m_Window, "GLFW window failed to be created.");
        glfwMakeContextCurrent(m_Window);

#ifdef RB_DEBUG
        glfwSetErrorCallback(glfwErrorCallbackFunc);
#endif
        glfwSwapInterval((int)m_VSync);

        // Window Close
        glfwSetWindowCloseCallback(m_Window,
            [](GLFWwindow* window)
            {
                (void)window;
                App& inst = App::getInstance();
                inst.onWindowClose();
                inst.close();
            });


        // Window Resize
        glfwSetWindowSizeCallback(m_Window,
            [](GLFWwindow* window, int width, int height)
            {
                (void)window;
                App& inst = App::getInstance();
                inst.getWindow().setWindowSize((uint16_t)width, (uint16_t)height);
                inst.onWindowResize((uint16_t)width, (uint16_t)height);
            });

        // Window Focused/Unfocused
        glfwSetWindowFocusCallback(m_Window,
            [](GLFWwindow* window, int focused)
            {
                (void)window;
                App& inst = App::getInstance();
                inst.getWindow().setFocused((bool)focused);
                inst.onWindowChangeFocus((bool)focused);
            });

        // Key Event
        glfwSetKeyCallback(m_Window,
            [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                (void)window;
                App& inst = App::getInstance();
                inst.onKeyEvent((KeyCode)key, scancode, (KeyAction)action, mods);
            });

        // Mouse Button
        glfwSetMouseButtonCallback(m_Window,
            [](GLFWwindow* window, int button, int action, int mods)
            {
                (void)window;
                App& inst = App::getInstance();
                inst.onMouseButton((MouseCode)button, (MouseAction)action, mods);
            });

        // Cursor Moved
        glfwSetCursorPosCallback(m_Window,
            [](GLFWwindow* window, double xpos, double ypos)
            {
                (void)window;
                App& inst = App::getInstance();
                inst.onMouseMove(xpos, ypos);
            });

        // Mouse Scrolled
        glfwSetScrollCallback(m_Window,
            [](GLFWwindow* window, double xoffset, double yoffset)
            {
                (void)window;
                App& inst = App::getInstance();
                inst.onMouseScroll(xoffset, yoffset);
            });

    }

    Window::~Window()
    {
        if(m_Window)
        {
            --s_WindowCount;
            glfwDestroyWindow(m_Window);
        }
        if(!s_WindowCount)
            glfwTerminate();
    }

    void Window::newFrame()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
        m_Minimized = (bool)glfwGetWindowAttrib(m_Window, GLFW_ICONIFIED);
    }

    void Window::setVSync(bool enabled)
    {
        glfwSwapInterval((int)enabled);
        m_VSync = enabled;
    }

    void Window::setWindowTitle(const char* title)
    {
        glfwSetWindowTitle(m_Window, title);
        m_Title = title;
    }

    void Window::setMouseLocked(bool locked)
    {
        glfwSetInputMode(m_Window, GLFW_CURSOR, locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    void Window::getCursorPos(double* o_MouseX, double* o_MouseY)
    {
        glfwGetCursorPos(m_Window, o_MouseX, o_MouseY);
    }

    void* Window::getNativeWindowPtr() const
    {
#ifdef RB_PLAT_WIND
        return (void*)glfwGetWin32Window(m_Window);
#elif RB_PLAT_LINUX
        //TODO: Add Linux version.
        return nullptr;
#endif
    }


    bool Window::isKeyDown(KeyCode code) const
    {
        return glfwGetKey(m_Window, (int)code) == GLFW_PRESS;
    }

    bool Window::isMouseButtonDown(MouseCode code) const
    {
        return glfwGetMouseButton(m_Window, (int)code) == GLFW_PRESS;
    }

    bool Window::isHovered() const
    {
        return (bool)glfwGetWindowAttrib(m_Window, GLFW_HOVERED);
    }

    Window& Window::operator=(Window&& other)
    {
        m_Window = other.m_Window;
        other.m_Window = nullptr;
        m_Title = std::move(other.m_Title);
        m_Width = other.m_Width;
        m_Height = other.m_Height;
        m_Minimized = other.m_Minimized;
        m_VSync = other.m_VSync;

        return *this;
    }

}
