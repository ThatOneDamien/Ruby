#pragma once

#include "Ruby/Main/Core.h"
#include "Ruby/Render/Framebuffer.h"

namespace Ruby 
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(int width, int height);
        virtual ~OpenGLFramebuffer();

        virtual void bind() const override;
        virtual void unbind() const override;
        virtual inline uint32_t getTextureID() const override { return m_ColorTextureID; }

        virtual void regenerate(int width, int height) override;
    private:
        RendererID m_RendererID = 0;
        uint32_t m_ColorTextureID = 0;
        int m_Width, m_Height;
    };


}
