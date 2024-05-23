#include "ruby_pch.h"

#ifdef RB_PLAT_WIND
#include "PlatformSpecific/Windows/WindowsWindow.h"
#define WINDOW WindowsWindow
#endif

namespace Ruby {

	UniPtr<Window> Window::createWindow(const WindowSpec& spec)
	{
		return createUniPtr<WINDOW>(spec);
	}

}