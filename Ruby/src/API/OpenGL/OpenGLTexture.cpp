#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Ruby/Main/App.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Ruby {

	namespace Internal 
	{
		inline GLuint pixelFormatToOpenGLInternal(PixelFormat format)
		{
			return (uint32_t)format >> 3;
		}

		inline GLuint pixelFormatToOpenGLBase(PixelFormat format)
		{
			switch (format)
			{
			case PixelFormat::R8:
				return GL_RED;
			case PixelFormat::RG8:
				return GL_RG;
			case PixelFormat::RGB8:
				return GL_RGB;
			case PixelFormat::RGBA8:
				return GL_RGBA;
			}
			RB_ERROR_DEBUG("Shouldn't be here.");
			return 0;
		}

		inline uint32_t bppFromPixelFormat(PixelFormat format)
		{
			return (uint32_t)format & 7;
		}

	}

	OpenGLTexture::OpenGLTexture(const std::string& filepath)
		: m_BoundSlot(-1)
	{
		stbi_set_flip_vertically_on_load(1);
		m_FormatIntern = 0;
		m_FormatBase = 0;
		uint8_t* data = nullptr;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		data = stbi_load(filepath.c_str(), (int*)(&m_Width), (int*)(&m_Height), (int*)(&m_BPP), 0);

		RB_ASSERT(data, "Unable to load texture at path %s", filepath.c_str());

		switch(m_BPP)
		{
		case 4:
			m_FormatIntern = GL_RGBA8;
			m_FormatBase = GL_RGBA;
			break;
		case 3:
			m_FormatIntern = GL_RGB8;
			m_FormatBase = GL_RGB;
			break;
		case 2:
			m_FormatIntern = GL_RG8;
			m_FormatBase = GL_RG;
			break;
		case 1:
			m_FormatIntern = GL_R8;
			m_FormatBase = GL_RED;
			break;
		default:
			RB_ERROR_DEBUG("Unknown or unsupported amount bytes per pixel.");
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, (m_BPP & 1) ? 1 : m_BPP);
		glTextureStorage2D(m_RendererID, 1, m_FormatIntern, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_FormatBase, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture::OpenGLTexture(const TextureSpec& spec)
		: m_Width(spec.Width), m_Height(spec.Height), m_BoundSlot(-1)
	{
		constexpr size_t MAX_TEX_SIZE = 4096UL * 4096UL;
		RB_ASSERT(((size_t)m_Width * (size_t)m_Height) <= MAX_TEX_SIZE, "Size of desired texture exceeds maximum allowed size.");
		RB_ASSERT(m_Width && m_Height, "Values of 0 are not allowed for width and height.");

		m_FormatIntern = Internal::pixelFormatToOpenGLInternal(spec.Format);
		m_FormatBase = Internal::pixelFormatToOpenGLBase(spec.Format);
		m_BPP = Internal::bppFromPixelFormat(spec.Format);

		glPixelStorei(GL_UNPACK_ALIGNMENT, (m_BPP & 1) ? 1 : m_BPP);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_FormatIntern, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, (GLint)spec.MinFilter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, (GLint)spec.MagFilter);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, (GLint)spec.WrapS);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, (GLint)spec.WrapT);
	}
	
	
	OpenGLTexture::OpenGLTexture(const void* data, const TextureSpec& spec)
		: m_Width(spec.Width), m_Height(spec.Height), m_BoundSlot(-1)
	{
		constexpr size_t MAX_TEX_SIZE = 4096UL * 4096UL;
		RB_ASSERT(((size_t)m_Width * (size_t)m_Height) <= MAX_TEX_SIZE, "Size of desired texture exceeds maximum allowed size.");
		RB_ASSERT(m_Width && m_Height, "Values of 0 are not allowed for width and height.");

		m_FormatIntern = Internal::pixelFormatToOpenGLInternal(spec.Format);
		m_FormatBase = Internal::pixelFormatToOpenGLBase(spec.Format);
		m_BPP = Internal::bppFromPixelFormat(spec.Format);

		glPixelStorei(GL_UNPACK_ALIGNMENT, (m_BPP & 1) ? 1 : m_BPP);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_FormatIntern, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, (GLint)spec.MinFilter);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, (GLint)spec.MagFilter);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, (GLint)spec.WrapS);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, (GLint)spec.WrapT);
		
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_FormatBase, GL_UNSIGNED_BYTE, data);
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

	void OpenGLTexture::setData(const void* data, uint32_t size)
	{
		RB_ASSERT(size == m_Width * m_Height * m_BPP, "Size of texture and size of data do not match.");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_FormatBase, GL_UNSIGNED_BYTE, data);
	}

}