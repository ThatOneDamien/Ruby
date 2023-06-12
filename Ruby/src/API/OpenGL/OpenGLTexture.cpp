#include "ruby_pch.h"

#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Ruby {

	OpenGLTexture::OpenGLTexture(const std::string& filepath)
		: m_BoundSlot(-1)
	{
		stbi_set_flip_vertically_on_load(1);
		m_FormatIntern = 0;
		m_FormatRender = 0;
		uint8_t* data = nullptr;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		data = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 0);

		if (!data)
		{
			RB_ERROR("Unable to load texture at path %s", filepath.c_str());
			return;
		}

		if (m_BPP == 4)
		{
			m_FormatIntern = GL_RGBA8;
			m_FormatRender = GL_RGBA;
		}
		else if (m_BPP == 3)
		{
			m_FormatIntern = GL_RGB8;
			m_FormatRender = GL_RGB;
		}
		else
			RB_ASSERT(false, "Unsupported amount of channels/bytes per pixel.");

		glTextureStorage2D(m_RendererID, 1, m_FormatIntern, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_FormatRender, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture::OpenGLTexture(int width, int height)
		: m_Width(width), m_Height(height), m_BoundSlot(-1)
	{
		// No negative values. Parameters are ints because that is what stb_image
		// takes as params for load func. Could use unsigned int but it works like
		// this too as I would still have to check the size of the texture anyway.
		RB_ASSERT(~(width >> 31 | height >> 31), "Negative width/height not allowed.");

		m_FormatIntern = GL_RGBA8;
		m_FormatRender = GL_RGBA;
		m_BPP = 4;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_FormatIntern, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	OpenGLTexture::~OpenGLTexture()
	{
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
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_FormatRender, GL_UNSIGNED_BYTE, data);
	}

}