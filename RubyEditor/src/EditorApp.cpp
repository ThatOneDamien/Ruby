#include <Ruby.h>
#include <Ruby/Main/MainFunctionEntry.h>
#include "EditorLayer.h"

namespace Ruby {

	class EditorApp : public App
	{
	public:
		EditorApp(int argc, char** argv, const std::string& mainDirectory, const char* name, int width, int height)
			: App(argc, argv, mainDirectory, name, width, height)
		{
			pushLayer(new EditorLayer());
		}
	};
}

Ruby::App* createApp(int argc, char** argv)
{
	return new Ruby::EditorApp(argc, argv, ".", "Ruby Editor", 1280, 720);
}