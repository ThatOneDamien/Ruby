#pragma once

#include "Window.h"

namespace Ruby 
{
    struct AppSpec
    {
        // Path to app's main directory relative to the BINARY
        // this may need to change between development and deployment.
        std::string MainDirectory;
        // Path to main Ruby directory relative to the MAIN DIRECTORY
        // specified in this struct. I know this is confusing, but it
        // is structured this way to avoid unnecessary copies of core
        // assets during development.
        std::string RubyDirectory;
        WindowSpec WinSpec
        {
            "Ruby Window", // Default Window Name
            960,           // Default Width
            540,           // Default Height
            128,           // Default Minimum Width
            72,            // Default Minimim Height
            true,          // Default VSync ON
            true           // Default TitleBar ON
        };
        
        API DesiredAPI = API::OpenGL;
    };

    class App
    {
    public:
        App() = delete;
        App(int argc, char** argv, const AppSpec& spec);
        virtual ~App();

        // Unmodifiable methods from base class App

        // Begins game loop, runs updates, and clocks frame time.
        // Calls user defined update() function every frame.
        void run();
        void close();


        // Methods to be defined by users app.

        // User defined function that is called upon the app being started.
        virtual void onStart() = 0;
        // User defined update handling function.
        virtual void update(double deltaMillis) = 0;
        // User defined ImGui rendering function.
        virtual void ImGuiRender(double deltaMillis) = 0;
        // User defined function that is called upon the app being closed.
        virtual void onExit() = 0;

        virtual void onWindowClose() {}
        virtual void onWindowResize(uint16_t width, uint16_t height) {}
        virtual void onWindowChangeFocus(bool focused) {}
        virtual void onKeyEvent(KeyCode code, int scancode, KeyAction action, int mods) {}
        virtual void onMouseButton(MouseCode code, MouseAction action, int mods) {}
        virtual void onMouseMove(double xpos, double ypos) {}
        virtual void onMouseScroll(double xoff, double yoff) {}


        // Get reference to window attached to this application
        inline Window& getWindow() { return m_Window; }
        // Get const reference to window attached to this application
        inline const Window& getWindow() const { return m_Window; }
        inline const std::string& getRubyDir() const { return m_RubyDir; }
        // Get reference to the current instance of this application
        static inline App& getInstance() { return *s_Instance; }
        // Returns true if there is an instance of the application running.
        static inline bool instanceExists() { return s_Instance; }

    private:
        static App* s_Instance;

    private:
        Window      m_Window;
        DeltaTime   m_DeltaT;
        std::string m_MainDir; 
        std::string m_RubyDir;
        char**      m_Argv{ nullptr };
        int         m_Argc{ 0 };
        bool        m_Running{ false };
    };


}
