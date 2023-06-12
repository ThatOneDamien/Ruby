#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Ruby/Render/Renderer.h"

#ifdef RB_USE_OPENGL
#define RB_RENDERER_GL
#include "API/OpenGL/OpenGLRenderer.h"
#endif