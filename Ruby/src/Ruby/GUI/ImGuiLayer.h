#pragma once

#include "Ruby/Main/Core.h"

#ifdef RB_USE_OPENGL
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#pragma warning(push, 0)
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#pragma warning(pop)
#else
#error "No other renderering APIs supported at the moment."
#endif


namespace Ruby {

	namespace ImGuiLayer {

		void init();
		void deInit();
		void begin();
		void end();

	}

}