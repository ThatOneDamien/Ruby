#include "ruby_pch.h"

#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Ruby {

	OpenGLVA::OpenGLVA()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVA::~OpenGLVA()
	{
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

	void OpenGLVA::pushVertexBuffer(const SharedPtr<VertexBuffer>& buffer)
	{
		glBindVertexArray(m_RendererID);
		buffer->bind();

		const VertexLayout& layout = buffer->getLayout();
		const std::vector<VertexLayoutElement>& e = layout.getElements();

		uint64_t offset = 0;
		for (GLuint i = 0; i < (GLuint)e.size(); i++)
		{
			glEnableVertexArrayAttrib(m_RendererID, i);
			switch (e[i].type)
			{
			case LayoutType::Byte:
				glVertexAttribPointer(i, e[i].count, GL_BYTE, e[i].normalize, layout.getStride(), (const void*)offset);
				break;
			case LayoutType::UByte:
				glVertexAttribPointer(i, e[i].count, GL_UNSIGNED_BYTE, e[i].normalize, layout.getStride(), (const void*)offset);
				break;
			case LayoutType::Short:
				glVertexAttribPointer(i, e[i].count, GL_SHORT, e[i].normalize, layout.getStride(), (const void*)offset);
				break;
			case LayoutType::UShort:
				glVertexAttribPointer(i, e[i].count, GL_UNSIGNED_SHORT, e[i].normalize, layout.getStride(), (const void*)offset);
				break;
			case LayoutType::Int:
				glVertexAttribPointer(i, e[i].count, GL_INT, e[i].normalize, layout.getStride(), (const void*)offset);
				break;
			case LayoutType::UInt:
				glVertexAttribPointer(i, e[i].count, GL_UNSIGNED_INT, e[i].normalize, layout.getStride(), (const void*)offset);
				break;
			case LayoutType::Float:
				glVertexAttribPointer(i, e[i].count, GL_FLOAT, (GLboolean)e[i].normalize, layout.getStride(), (const void*)offset);
				break;
			case LayoutType::Double:
				glVertexAttribPointer(i, e[i].count, GL_DOUBLE, e[i].normalize, layout.getStride(), (const void*)offset);
				break;
			default:
				RB_ASSERT(false, "Unsupported data type!");
			}
			offset += e[i].count * sizeOfLayoutType(e[i].type);
		}

		m_VertexBuffers.push_back(buffer);
	}

	void OpenGLVA::setIndexBuffer(const SharedPtr<IndexBuffer>& buffer)
	{
		glBindVertexArray(m_RendererID);
		buffer->bind();

		m_IndexBuffer = buffer;
	}

}