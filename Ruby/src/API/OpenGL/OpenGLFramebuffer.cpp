#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Ruby/Main/App.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Ruby 
{
    OpenGLFramebuffer::OpenGLFramebuffer(int width, int height)
        : m_Width(width), m_Height(height)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorTextureID);

        glBindTexture(GL_TEXTURE_2D, m_ColorTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTextureID, 0);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        RB_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete!");
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        if(App::instanceExists())
        {
            glDeleteTextures(1, &m_ColorTextureID);
            glDeleteFramebuffers(1, &m_RendererID);
        }
    }

    void OpenGLFramebuffer::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    }

    void OpenGLFramebuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::regenerate(int width, int height)
    {
        glDeleteTextures(1, &m_ColorTextureID);
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorTextureID);

        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_ColorTextureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTextureID, 0);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        RB_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_Width = width;
        m_Height = height;
    }

}