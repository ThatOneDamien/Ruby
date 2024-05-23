#include "ruby_pch.h"

#include "Core.h"

#ifdef RB_PLAT_WIND
#include "PlatformSpecific/Windows/WindowsWindow.h"
#define WINDOW WindowsWindow
#elif RB_PLAT_LINUX
#include "PlatformSpecific/Linux/LinuxWindow.h"
#define WINDOW LinuxWindow
#endif

namespace Ruby {

	UniPtr<Window> Window::createWindow(const WindowSpec& spec)
	{
		return createUniPtr<WINDOW>(spec);
	}

}
