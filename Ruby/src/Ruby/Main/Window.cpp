#include "ruby_pch.h"

#ifdef RB_PLAT_WIND
#include "PlatformSpecific/Windows/WindowsWindow.h"
#define WINDOW WindowsWindow
#endif

namespace Ruby {

	UniPtr<Window> Window::createWindow(const char* name, uint16_t width, uint16_t height, bool vSync)
	{
		return createUniPtr<WINDOW>(name, width, height, vSync);
	}

}