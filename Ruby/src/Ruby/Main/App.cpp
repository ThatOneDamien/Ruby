#include "ruby_pch.h"

#include "App.h"
#include "Core.h"
#include "Ruby/Render/Renderer.h"
#include "Ruby/Audio/Audio.h"
#include "Ruby/Render/Font.h"
#include "Ruby/Event/AppEvent.h"
#include "Ruby/GUI/ImGuiUtil.h"

#include <filesystem>

namespace Ruby {

	App* App::s_Instance = nullptr;

	App::App(int argc, char** argv, const AppSpec& spec)
		: m_Argc(argc), m_Argv(argv), m_MainDir(spec.MainDirectory)
	{
		RB_ASSERT(!s_Instance, "An instance of App already exists in the program.");
		s_Instance = this;
		m_Running = true;
		if (!m_MainDir.empty())
			std::filesystem::current_path(m_MainDir);

		// Create window and initialize windowing library.

		m_Window = Window::createWindow(spec.WindowSpec);
		
		Font::init();
		Audio::init();

		// Initialize the renderer, depending on API this will incorporate the windowing library.
		Renderer::init();
		ImGuiUtil::init();
	}

	App::~App()
	{
		ImGuiUtil::deInit();
		Renderer::deInit();
		Ruby::Audio::deInit();
		Ruby::Font::deInit();
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
				Renderer::API::setViewport(0, 0, (int)width, (int)height);
			}
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