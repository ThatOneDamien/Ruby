#pragma once
#ifndef RB_EXTERNAL_DEF
#error "Include Ruby.h"
#elif (defined RB_PLAT_WIND)
Ruby::App* createApp(int argc, char** argv);

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <PlatformSpecific/Windows/WindowsCommandLine.h>

int APIENTRY WinMain(_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     PSTR     pCmdLine,
	_In_     int       nCmdShow)
{
	// Generate console window only if debugging
#ifdef RB_DEBUG
	if (!Ruby::CL::CreateNewConsole(1024))
		return -1;
#endif


	Ruby::Logger::init();
	Ruby::Time::init();
	Ruby::App* app = createApp(__argc, __argv);
	app->run();
	delete app;


#ifdef RB_DEBUG
	Ruby::CL::ReleaseConsole();
#endif


	return 0;
}

#else
Ruby::App* createApp(int argc, char** argv);

int main(int argc, char** argv)
{
	Ruby::Logger::init();
	Ruby::Time::init();
	Ruby::App* app = createApp(argc, argv);
	app->run();
	delete app;
	return 0;
}

#endif