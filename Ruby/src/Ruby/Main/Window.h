#pragma once

#include "Core.h"
#include "Ruby/Render/Context.h"

#include "MouseCodes.h"
#include "KeyCodes.h"

// Forward Declaration
struct GLFWwindow;
typedef int Bruh;

namespace Ruby 
{
    struct WindowSpec
    {
        const char* Name;
        int Width;
        int Height;
        int MinWidth;
        int MinHeight;
        bool VSync;
        bool HasTitleBar;
    };

    class Window
    {
    public:
        Window() = default;
        Window(const Window&) = delete;
        Window(const WindowSpec& spec, API desiredAPI);
        ~Window();

        void newFrame();
        void setVSync(bool enabled);
        void setWindowTitle(const char* title);
        void setMouseLocked(bool locked);
        inline void setFocused(bool focused) { m_Focused = focused; }
        inline void setWindowSize(uint16_t new_width, uint16_t new_height) 
        {
            m_Width = new_width; 
            m_Height = new_height; 
            m_Minimized = m_Width == 0;
        }

        inline void* getWinPointer() const         { return m_Window; }
        inline uint16_t getWidth() const           { return m_Width; }
        inline uint16_t getHeight() const          { return m_Height; }
        inline const std::string& getTitle() const { return m_Title; }
        inline float getAspectRatio() const        { return (float)m_Width / (float)m_Height; }

        void getCursorPos(double* o_MouseX, double* o_MouseY);
        void* getNativeWindowPtr() const;

        bool isKeyDown(KeyCode code) const;
        bool isMouseButtonDown(MouseCode code) const;
        bool isHovered() const;
        inline bool isMinimized() const { return m_Minimized; }
        inline bool isFocused() const { return m_Focused; }
        inline bool isVSynced() const { return m_VSync; } 

        Window& operator=(Window&&);

    private:
        GLFWwindow* m_Window{ nullptr };
        std::string m_Title;
        uint16_t    m_Width{ 0 };
        uint16_t    m_Height{ 0 };
        bool        m_VSync{ false };
        bool        m_Minimized{ false };
        bool        m_Focused{ false };
    };
}
