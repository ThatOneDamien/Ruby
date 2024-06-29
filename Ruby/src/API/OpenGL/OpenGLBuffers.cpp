#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Ruby/Main/App.h"
#include "OpenGLBuffers.h"

#include <glad/glad.h>

namespace Ruby 
{
    /*
    * 
    * --------------VERTEX BUFFER----------------
    * 
    */

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
        if(App::instanceExists())
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
        RB_ASSERT_RET_VOID(size + offset <= m_Size, "Size of given data and/or offset would overflow the size of VBO.");
        glNamedBufferSubData(m_RendererID, (GLintptr)offset, (GLsizeiptr)size, vertices);
    }






    /*
    *
    * --------------INDEX BUFFER----------------
    *
    */

    OpenGLIB::OpenGLIB(const uint32_t* indices, uint32_t count)
        : m_Count(count)
    {
        glCreateBuffers(1, &m_RendererID);
        glNamedBufferData(m_RendererID, count * sizeof(int), (const void*)indices, GL_STATIC_DRAW);
    }

    OpenGLIB::~OpenGLIB()
    {
        if(App::instanceExists())
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






    /*
    *
    * --------------UNIFORM BUFFER----------------
    *
    */


    OpenGLUB::OpenGLUB(const void* data, uint32_t size, uint8_t binding)
        : m_Size(size), m_Binding(binding)
    {
        glCreateBuffers(1, &m_RendererID);
        glNamedBufferData(m_RendererID, size, data, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
    }

    OpenGLUB::OpenGLUB(uint32_t size, uint8_t binding)
        : m_Size(size), m_Binding(binding)
    {
        glCreateBuffers(1, &m_RendererID);
        glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
    }

    OpenGLUB::~OpenGLUB()
    {
        if(App::instanceExists())
            glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLUB::bind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
    }

    void OpenGLUB::unbind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void OpenGLUB::setData(const void* data, uint32_t size, uint32_t offset)
    {
        glNamedBufferSubData(m_RendererID, (GLintptr)offset, (GLsizeiptr)size, data);
    }



}
