#pragma once

#include "Window.h"

#include "Ruby/Event/Event.h"

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
        App(int argc, char** argv, const AppSpec& spec);
        virtual ~App();

        // Unmodifiable methods from base class App

        // Handles event e generated from the window based on user input.
        // This function handles basic window events by forwarding information
        // to the graphics API, and then sends this event to the user defined
        // onEvent() function.
        void handleEvent(Event& e);

        // Begins game loop, runs updates, and clocks frame time.
        // Calls user defined update() function every frame.
        void run();


        // Methods to be defined by users app.

        // User defined function that is called upon the app being started.
        virtual void onStart() = 0;
        // User defined update handling function.
        virtual void update(double deltaMillis) = 0;
        // User defined ImGui rendering function.
        virtual void ImGuiRender(double deltaMillis) = 0;
        // User defined event handling function.
        virtual void onEvent(Event& e) = 0;
        // User defined function that is called upon the app being closed.
        virtual void onExit() = 0;

        // Get reference to window attached to this application
        inline Window& getWindow() { return *m_Window; }
        // Get const reference to window attached to this application
        inline const Window& getWindow() const { return *m_Window; }
        inline const std::string& getRubyDir() const { return m_RubyDir; }
        // Get reference to the current instance of this application
        static inline App& getInstance() { return *s_Instance; }
        // Returns true if there is an instance of the application running.
        static inline bool instanceExists() { return s_Instance; }

    private:

        bool m_Running = false, m_Minimized = false;
        int m_Argc;
        char** m_Argv;
        std::string m_MainDir, m_RubyDir;

        static App* s_Instance;
        Window* m_Window;
        DeltaTime m_DT{};


    };


}
