#include "ruby_pch.h"

#include "Ruby/Main/Core.h"

#ifdef RB_USE_OPENGL
#include "API/OpenGL/OpenGLFramebuffer.h"
#define FRAMEBUFFER OpenGLFramebuffer
#endif

namespace Ruby 
{
	SharedPtr<Framebuffer> Framebuffer::create(int width, int height)
	{
		return createShared<FRAMEBUFFER>(width, height);
	}

}