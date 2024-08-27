#include "ruby_pch.h"

#include "App.h"
#include "Core.h"
#include "Ruby/Render/Context.h"
#include "Ruby/Render/Renderer2D.h"
#include "Ruby/Audio/Audio.h"
#include "Ruby/Render/Font.h"
#include "Ruby/Event/AppEvent.h"
#include "Ruby/GUI/ImGuiUtil.h"

#include <filesystem>

namespace Ruby 
{
    // Forward declarations
    namespace Audio { void init(); void deInit(); }

    App* App::s_Instance = nullptr;

    App::App(int argc, char** argv, const AppSpec& spec)
        : m_Argc(argc), m_Argv(argv), m_MainDir(spec.MainDirectory), m_RubyDir(spec.RubyDirectory)
    {
        RB_ASSERT(!s_Instance, "An instance of App already exists in the program.");
        s_Instance = this;
        m_Running = true;

        // Set working directory relative to the binary location.
        RB_ASSERT(argc > 0, "Argc is <=0???");
        std::filesystem::path binaryParentDir = std::filesystem::path(argv[0]).parent_path();
        std::filesystem::current_path(binaryParentDir);

        if (!m_MainDir.empty())
            std::filesystem::current_path(spec.MainDirectory);
        
        RB_INFO("Working directory set at: \'%s\'", std::filesystem::current_path().string().c_str());

        // Create window and initialize windowing library.
        m_Window = new Window(spec.WinSpec, spec.DesiredAPI);
        
        Font::init();
        Audio::init();

        // Init the Rendering API
        Context::initAPI(spec.DesiredAPI);
        ImGuiUtil::init();
    }

    App::~App()
    {
        if(s_Instance == this)
            s_Instance = nullptr;
            
        Ruby::ImGuiUtil::deInit();
        Ruby::Audio::deInit();
        Ruby::Font::deInit();

        delete m_Window;
    }

    void App::handleEvent(Event& e)
    {
        switch (e.getType())
        {
            case EventType::WindowClose:
                m_Running = false;
                return;
            case EventType::WindowResized:
            {
                WindowResizeEvent& event = static_cast<WindowResizeEvent&>(e);
                uint32_t width = event.getWidth(), height = event.getHeight();
                m_Minimized = width == 0;
                if (m_Minimized)
                    return;
                m_Minimized = false;
                m_Window->windowResized(width, height);
                Context::setViewport(0, 0, (int)width, (int)height);
            }
            default:
                break;
        }

        onEvent(e);
    }

    void App::run()
    {
        onStart();
        while (m_Running)
        {
            double deltaMillis = m_DT.getMillisAndClock();

            if (!m_Minimized)
            {
                update(deltaMillis);

                ImGuiUtil::begin();
                ImGuiRender(deltaMillis);
                ImGuiUtil::end();
            }

            m_Window->update();
        }
        onExit();
    }

}
