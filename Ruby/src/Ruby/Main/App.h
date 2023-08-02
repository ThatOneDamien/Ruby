#pragma once

#include "Layer.h"
#include "Window.h"

#include "Ruby/Event/Event.h"

namespace Ruby {

	class App
	{
	public:
		App(int argc, char** argv, const std::string& mainDir, const char* name, int width, int height);
		virtual ~App();

		void onEvent(Event& e);
		void run();
		inline void pushLayer(Layer* layer) { m_LayerStack.pushLayer(layer); layer->onPush(); }
		inline void popLayer(Layer* layer) { m_LayerStack.popLayer(layer); layer->onPop(); }
		inline void pushOverlay(Layer* overlay) { m_LayerStack.pushOverlay(overlay); overlay->onPush(); }
		inline void popOverlay(Layer* overlay) { m_LayerStack.popOverlay(overlay); overlay->onPop(); }

		inline Window& getWindow() { return *m_Window; }
		inline const Window& getWindow() const { return *m_Window; }
		static inline App& getInstance() { return *s_Instance; }

	private:

		bool m_Running = false, m_Minimized = false;
		int m_Argc;
		char** m_Argv;
		std::string m_MainDir;

		static App* s_Instance;
		UniPtr<Window> m_Window;
		LayerStack m_LayerStack;
		DeltaTime m_DT;


	};


}