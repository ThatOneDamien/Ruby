#include "ruby_pch.h"

#include "OpenGLBuffers.h"
#include "Ruby/Main/Core.h"

#include <glad/glad.h>

namespace Ruby {

	OpenGLVB::OpenGLVB(const void* vertices, uint32_t size)
		: m_Size(size)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVB::OpenGLVB(uint32_t size)
		: m_Size(size)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVB::~OpenGLVB()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVB::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVB::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

	void OpenGLVB::setVertexData(const void* vertices, uint32_t size, uint32_t offset)
	{
		if (size + offset > m_Size)
		{
			RB_ERROR("Size of given data and/or offset would overflow the size of VBO.");
			return;
		}
		glNamedBufferSubData(m_RendererID, (GLintptr)offset, (GLsizeiptr)size, vertices);
	}

	OpenGLIB::OpenGLIB(const uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, count * sizeof(int), (const void*)indices, GL_STATIC_DRAW);
	}

	OpenGLIB::~OpenGLIB()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIB::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIB::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}