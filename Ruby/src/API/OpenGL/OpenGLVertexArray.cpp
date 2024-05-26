#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Ruby/Main/App.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Ruby 
{
	OpenGLVA::OpenGLVA()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVA::~OpenGLVA()
	{
		if(App::instanceExists())
			glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVA::bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVA::unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVA::setVertexBuffer(const SharedPtr<VertexBuffer>& buffer)
	{
		glBindVertexArray(m_RendererID);
		buffer->bind();

		const VertexLayout& layout = buffer->getLayout();
		const std::vector<VertexLayoutElement>& e = layout.getElements();

		uint64_t offset = 0;
		for (GLuint i = 0; i < (GLuint)e.size(); ++i)
		{
			glEnableVertexArrayAttrib(m_RendererID, i);

			// A little bit of magic using the encoding of the enums.
			GLenum glType = 0x1400u + ((uint32_t)e[i].type >> 4);
			glVertexAttribPointer(i, e[i].count, glType, e[i].normalize, layout.getStride(), (const void*)offset);
			offset += (uint64_t)e[i].count * (uint64_t)sizeOfLayoutType(e[i].type);
		}

		m_VertexBuffer = buffer;
	}

	void OpenGLVA::setIndexBuffer(const SharedPtr<IndexBuffer>& buffer)
	{
		glBindVertexArray(m_RendererID);
		buffer->bind();

		m_IndexBuffer = buffer;
	}

}