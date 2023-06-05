#pragma once

#include "Ruby/Main/Window.h"
#include "Ruby/Event/Event.h"

namespace Ruby {

	class App
	{
	public:
		App();
		virtual ~App();

		void onEvent(Event& e);
		void run();
		static inline App& getInstance() { return *s_Instance; }

	private:
		bool m_Running = false;
		static App* s_Instance;
		std::unique_ptr<Window> m_Window;
	};


}