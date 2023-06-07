#include "ruby_pch.h"

#include "Ruby/Main/Core.h"

#include "ImGuiLayer.h"

#pragma warning(push, 0)
#ifdef RB_USE_OPENGL
#include "API/OpenGL/OpenGLImGui_Impl.h"
#endif
#pragma warning(pop)