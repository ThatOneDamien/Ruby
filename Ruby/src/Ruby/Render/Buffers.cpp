#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Ruby/Render/Buffers.h"
#ifdef RB_USE_OPENGL
#include "API/OpenGL/OpenGLBuffers.h"
#define VBUFFER OpenGLVB
#define IBUFFER OpenGLIB
#endif

namespace Ruby {

	SharedPtr<VertexBuffer> VertexBuffer::createVBO(const void* vertices, uint32_t size)
	{
		return createShared<VBUFFER>(vertices, size);
	}

	SharedPtr<VertexBuffer> VertexBuffer::createVBO(uint32_t size)
	{
		return createShared<VBUFFER>(size);
	}

	SharedPtr<IndexBuffer> IndexBuffer::createIBO(const uint32_t* indices, uint32_t count)
	{
		return createShared<IBUFFER>(indices, count);
	}

}