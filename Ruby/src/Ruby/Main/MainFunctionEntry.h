#pragma once
#ifndef RB_EXTERNAL_DEF
#error "Include Ruby.h"
#else
Ruby::App* createApp(int argc, char** argv);

int main(int argc, char** argv)
{
	Ruby::Logger::init();
	Ruby::Time::init();
	Ruby::App* app = createApp(argc, argv);
	app->run();
	delete app;
}

#endif