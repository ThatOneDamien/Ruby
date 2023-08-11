#include "ruby_pch.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Font.h"

namespace Ruby {

	static FT_Library s_Lib{ nullptr };

	Font::Font(const std::string& filepath, uint32_t pixelSize)
		: m_PixelSize(pixelSize)
	{
		RB_ASSERT(s_Lib, "FT Library not initialized, maybe you forgot to call Font::init()?");
		
		
		RB_ASSERT(!FT_New_Face(s_Lib, filepath.c_str(), 0, &m_FontFace), "Unable to load font '%s'.", filepath.c_str());
		FT_Set_Pixel_Sizes(m_FontFace, 0, m_PixelSize);

        for (uint8_t c = 0; c < 128; c++)
        {
            // load character glyph 
            if (FT_Load_Char(m_FontFace, c, FT_LOAD_RENDER))
            {
                RB_ERROR("Unable to load glyph %c", c);
                continue;
            }

            if (m_FontFace->glyph->bitmap.width == 0 || m_FontFace->glyph->bitmap.rows == 0)
                continue;

            TextureSpec spec;
            spec.Format = PixelFormat::R8;
            spec.Width = m_FontFace->glyph->bitmap.width;
            spec.Height = m_FontFace->glyph->bitmap.rows;
            spec.MagFilter = TextureFilter::Linear;
            spec.MinFilter = TextureFilter::Linear;
            spec.WrapS = TextureWrap::ClampToEdge;
            spec.WrapT = TextureWrap::ClampToEdge;

            // now store character for later use
            Character character = {
                Texture::createTexture((const void*)m_FontFace->glyph->bitmap.buffer, spec),
                glm::ivec2(m_FontFace->glyph->bitmap.width, m_FontFace->glyph->bitmap.rows),
                glm::ivec2(m_FontFace->glyph->bitmap_left, m_FontFace->glyph->bitmap_top),
                m_FontFace->glyph->advance.x
            };
            m_Characters.emplace(c, character);
        }
	}

	Font::~Font()
	{
		FT_Done_Face(m_FontFace);
	}

    void Font::init()
	{
		RB_ASSERT(!FT_Init_FreeType(&s_Lib), "Freetype failed to initialize!");
	}

	void Font::deInit()
	{
		FT_Done_FreeType(s_Lib);
	}

}