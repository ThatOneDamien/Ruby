#pragma once

#include "Core.h"
#include "Ruby/Render/Context.h"

#include "MouseCodes.h"
#include "KeyCodes.h"

// Forward Declaration
struct GLFWwindow;

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
        Window() = delete;
        Window(const Window&) = delete;
        Window(const WindowSpec& spec, API desiredAPI);
        ~Window();

        void update() const;
        void setVSync(bool enabled);
        void setWindowTitle(const char* title);
        inline void windowResized(uint16_t new_width, uint16_t new_height) { m_Width = new_width; m_Height = new_height; }

        inline void* getWinPointer() const { return m_Window; }
        inline uint16_t getWidth() const { return m_Width; }
        inline uint16_t getHeight() const { return m_Height; }
        inline const std::string& getTitle() const { return m_Title; }
        inline float getAspectRatio() const { return (float)m_Width / (float)m_Height; }
        void* getNativeWindowPtr() const;

        bool isKeyDown(KeyCode code) const;
        bool isMouseButtonDown(MouseCode code) const;
        inline bool isVSynced() const { return m_VSync; }

    private:
        GLFWwindow* m_Window = nullptr;
        std::string m_Title;
        uint16_t m_Width, m_Height;
        bool m_VSync;
        bool m_Minimized = false;
    };
}
