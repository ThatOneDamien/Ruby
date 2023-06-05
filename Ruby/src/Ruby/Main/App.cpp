#include "ruby_pch.h"

#include "Ruby/Main/App.h"
#include "Ruby/Render/Renderer.h"

#include "Ruby/Event/AppEvent.h"
#include "Ruby/Event/KeyEvent.h"
#include "Ruby/Event/MouseEvent.h"

namespace Ruby {

	App* App::s_Instance = nullptr;

	App::App()
	{
		RB_ASSERT(!s_Instance, "An instance of App already exists in the program.");
		s_Instance = this;
		m_Running = true;
		m_Window = Window::createWindow();
		Renderer::init();
		
	}

	App::~App()
	{

	}

	void App::onEvent(Event& e)
	{

		if (e.getType() == EventType::WindowClose)
			m_Running = false;
		else if (e.getType() == EventType::MouseScroll)
			RB_TRACE("Mouse Scroll Event, %f %f", ((MouseScrollEvent&)e).getXOffset(), ((MouseScrollEvent&)e).getYOffset());
	}

	void App::run()
	{
		Renderer::setClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		while (m_Running)
		{
			Renderer::clear();
			m_Window->update();
		}
	}

}