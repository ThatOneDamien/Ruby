#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "VertexArray.h"

#ifdef RB_USE_OPENGL
#include "API/OpenGL/OpenGLVertexArray.h"
#define VARRAY OpenGLVA
#endif

namespace Ruby 
{
	SharedPtr<VertexArray> VertexArray::createVAO()
	{
		return createShared<VARRAY>();
	}

}