#include "ruby_pch.h"

#include "Ruby/Main/App.h"
#include "Ruby/Render/Renderer.h"

#include "Ruby/Event/AppEvent.h"
#include "Ruby/Event/KeyEvent.h"
#include "Ruby/Event/MouseEvent.h"

#include "Ruby/GUI/ImGuiLayer.h"

namespace Ruby {

	App* App::s_Instance = nullptr;

	App::App(int argc, char** argv, const std::string& mainDir)
		: m_Argc(argc), m_Argv(argv), m_MainDir(mainDir)
	{
		RB_ASSERT(!s_Instance, "An instance of App already exists in the program.");
		s_Instance = this;
		m_Running = true;

		if (!m_MainDir.empty())
			std::filesystem::current_path(mainDir);

		// Create window and initialize windowing library.
		m_Window = Window::createWindow();
		
		// Initialize the renderer, depending on API this will incorporate the windowing library.
		Renderer::init();
		ImGuiLayer::init();

	}

	App::~App()
	{
		ImGuiLayer::deInit();
		Renderer::deInit();
	}

	void App::onEvent(Event& e)
	{
		if (e.getType() == EventType::WindowClose)
			m_Running = false;


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
		Renderer::setClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		while (m_Running)
		{
			Renderer::clear();

			for (size_t i = 0; i < m_LayerStack.size(); i++)
				m_LayerStack[i].update();

			ImGuiLayer::begin();
			for (size_t i = 0; i < m_LayerStack.size(); i++)
				m_LayerStack[i].ImGuiRender();
			ImGuiLayer::end();

			m_Window->update();
		}
	}

}