#include "ruby_pch.h"

#ifdef RB_PLAT_WIND
#include "PlatformSpecific/Windows/WindowsWindow.h"
#endif

namespace Ruby {

	UniPtr<Window> Window::createWindow(const char* name, uint16_t width, uint16_t height, bool vSync)
	{
	#ifdef RB_PLAT_WIND
		return createUniPtr<WindowsWindow>(name, width, height, vSync);
	#endif
	}

}