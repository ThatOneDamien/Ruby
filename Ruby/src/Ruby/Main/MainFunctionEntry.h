#pragma once
#ifndef RB_DEFINED
#error "Include Ruby.h"
#else
Ruby::App* createApp(int argc, char** argv);

int main(int argc, char** argv)
{
	::Ruby::Logger::init();
	::Ruby::Time::init();
	Ruby::App* app = createApp(argc, argv);
	app->run();
	delete app;
}

#endif