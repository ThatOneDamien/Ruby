#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Ruby/Render/Buffers.h"
#ifdef RB_USE_OPENGL
#include "API/OpenGL/OpenGLBuffers.h"
#define VBUFFER OpenGLVB
#define IBUFFER OpenGLIB
#endif

namespace Ruby {

	std::shared_ptr<VertexBuffer> VertexBuffer::createVBO(const void* vertices, uint32_t size)
	{
		return std::make_shared<VBUFFER>(vertices, size);
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::createVBO(uint32_t size)
	{
		return std::make_shared<VBUFFER>(size);
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::createIBO(const uint32_t* indices, uint32_t count)
	{
		return std::make_shared<IBUFFER>(indices, count);
	}

}