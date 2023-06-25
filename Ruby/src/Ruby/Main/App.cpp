#include "ruby_pch.h"

#include "Ruby/Main/App.h"
#include "Ruby/Render/Renderer.h"

#include "Ruby/Event/AppEvent.h"
#include "Ruby/Event/KeyEvent.h"
#include "Ruby/Event/MouseEvent.h"

#include "Ruby/GUI/ImGuiUtil.h"

namespace Ruby {

	App* App::s_Instance = nullptr;

	App::App(int argc, char** argv, const std::string& mainDir, const char* name, int width, int height)
		: m_Argc(argc), m_Argv(argv), m_MainDir(mainDir), m_DT(TimingPrecision::Seconds)
	{
		RB_ASSERT(!s_Instance, "An instance of App already exists in the program.");
		s_Instance = this;
		m_Running = true;

		if (!m_MainDir.empty())
			std::filesystem::current_path(mainDir);

		// Create window and initialize windowing library.
		m_Window = Window::createWindow(name, width, height);
		
		// Initialize the renderer, depending on API this will incorporate the windowing library.
		Renderer::init();
		ImGuiUtil::init();

	}

	App::~App()
	{
		ImGuiUtil::deInit();
		Renderer::deInit();
	}

	void App::onEvent(Event& e)
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
				if (width == 0)
				{
					m_Minimized = true;
					return;
				}
				m_Minimized = false;
				m_Window->windowResized(width, height);
				Renderer::API::setViewport(0, 0, (int)width, (int)height);
				break;
			}
		}


		// Loop through layer stack in reverse order until event is marked as handeled.
		for (size_t i = m_LayerStack.size(); i > 0; )
		{
			m_LayerStack[--i].onEvent(e);

			if (e.handeled)
				break;
		}
	}

	void App::run()
	{
		m_DT.getAndClock();
		Renderer::API::setClearColor(0.1f, 0.1f, 0.1f, 1.1f);
		while (m_Running)
		{
			double delta_time = m_DT.getAndClock();
			
			if (!m_Minimized)
			{
				for (size_t i = 0; i < m_LayerStack.size(); i++)
					m_LayerStack[i].update(delta_time);

				ImGuiUtil::begin();
				for (size_t i = 0; i < m_LayerStack.size(); i++)
					m_LayerStack[i].ImGuiRender();
				ImGuiUtil::end();
			}

			m_Window->update();
		}
	}

}