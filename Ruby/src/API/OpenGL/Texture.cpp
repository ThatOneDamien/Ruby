#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Ruby/Main/App.h"
#include "API/OpenGL/Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Ruby 
{
    static inline GLenum bppToInternal(int bpp)
    {
        RB_ASSERT(bpp >= 1 && bpp <= 4, "Invalid bits per pixel.");
        static const GLenum CONVERSION[4] = { GL_R8, GL_RG8, GL_RGB8, GL_RGBA8 };
        return CONVERSION[bpp - 1];
    }
    
    static inline GLenum bppToPixelDataFormat(int bpp)
    {
        RB_ASSERT(bpp >= 1 && bpp <= 4, "Invalid bits per pixel.");
        static const GLenum CONVERSION[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
        return CONVERSION[bpp - 1];
    }
    
    static constexpr size_t MAX_TEX_SIZE = 4096UL * 4096UL;

    OpenGLTexture::OpenGLTexture(const std::string& filepath, const TextureSpec& spec)
        : m_BoundSlot(-1), m_Filepath(filepath)
    {
        stbi_set_flip_vertically_on_load(true);
        uint8_t* data = nullptr;

        data = stbi_load(filepath.c_str(), (int*)(&m_Width), (int*)(&m_Height), (int*)(&m_BPP), (int)(spec.Format));
        if(spec.Format != PixelFormat::Any)
            m_BPP = (uint32_t)spec.Format;

        RB_ENSURE_RET_VOID(data && m_BPP, "Unable to load texture at path %s", filepath.c_str());

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, bppToInternal(m_BPP), m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, (GLint)spec.MinFilter);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, (GLint)spec.MagFilter);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, (GLint)spec.WrapS);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, (GLint)spec.WrapT);

        setData(data, m_Width * m_Height * m_BPP, (PixelFormat)m_BPP);

        stbi_image_free(data);
    }

    OpenGLTexture::OpenGLTexture(const TextureSpec& spec)
        : m_Width(spec.Width), m_Height(spec.Height), m_BoundSlot(-1)
    {
        RB_ASSERT(((size_t)m_Width * (size_t)m_Height) <= MAX_TEX_SIZE, "Size of desired texture exceeds maximum allowed size.");
        RB_ASSERT(m_Width && m_Height, "Values of 0 are not allowed for width and height.");

        RB_ASSERT(spec.Format != PixelFormat::Any, "Cannot provide \'any\' for empty texture.");
        m_BPP = (uint32_t)spec.Format;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, bppToInternal(m_BPP), m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, (GLint)spec.MinFilter);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, (GLint)spec.MagFilter);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, (GLint)spec.WrapS);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, (GLint)spec.WrapT);
    }
    
    
    OpenGLTexture::OpenGLTexture(const void* data, const TextureSpec& spec)
        : m_Width(spec.Width), m_Height(spec.Height), m_BoundSlot(-1)
    {
        RB_ASSERT(((size_t)m_Width * (size_t)m_Height) <= MAX_TEX_SIZE, "Size of desired texture exceeds maximum allowed size.");
        RB_ASSERT(m_Width && m_Height, "Values of 0 are not allowed for width and height.");

        RB_ASSERT(spec.Format != PixelFormat::Any, "Cannot provide \'any\' for raw data texture.");
        m_BPP = (uint32_t)spec.Format;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, bppToInternal(m_BPP), m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, (GLint)spec.MinFilter);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, (GLint)spec.MagFilter);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, (GLint)spec.WrapS);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, (GLint)spec.WrapT);
        
        setData(data, m_Width * m_Height * m_BPP, spec.Format);
    }

    OpenGLTexture::~OpenGLTexture()
    {
        if(App::instanceExists())
            glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture::bind(uint8_t slot) const
    {
        glBindTextureUnit(slot, m_RendererID);
        m_BoundSlot = slot;
    }

    void OpenGLTexture::unbind() const
    {
        glBindTextureUnit(m_BoundSlot, 0);
        m_BoundSlot = -1;
    }

    void OpenGLTexture::setData(const void* data, uint32_t size, PixelFormat format)
    {
        RB_ASSERT(size == m_Width * m_Height * m_BPP, "Size of texture and size of data do not match.");
        RB_ASSERT(format != PixelFormat::Any, "Cannot provide a pixel format of \'any\' when setting data.");
        glPixelStorei(GL_UNPACK_ALIGNMENT, (m_BPP & 1) ? 1 : m_BPP);
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, bppToPixelDataFormat((int)format), GL_UNSIGNED_BYTE, data);
    }

}
