#include "ruby_pch.h"

//#include <ft2build.h>
//#include FT_FREETYPE_H
//
//#include "Font.h"
//#include <glad/glad.h>
//
//namespace Ruby {
//
//	static FT_Library s_Lib{ nullptr };
//
//	Font::Font(const std::string& filepath, uint32_t pixelSize)
//		: m_PixelSize(pixelSize)
//	{
//		RB_ASSERT(s_Lib, "FT Library not initialized, maybe you forgot to call Font::init()?");
//		
//		
//		RB_ASSERT(!FT_New_Face(s_Lib, filepath.c_str(), 0, &m_FontFace), "Unable to load font '%s'.", filepath.c_str());
//		FT_Set_Pixel_Sizes(m_FontFace, 0, m_PixelSize);
//
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
//
//        for (uint8_t c = 0; c < 128; c++)
//        {
//            // load character glyph 
//            if (FT_Load_Char(m_FontFace, c, FT_LOAD_RENDER))
//            {
//                RB_ERROR("Unable to load glyph %c", c);
//                continue;
//            }
//            // generate texture
//            uint32_t texture;
//            glGenTextures(1, &texture);
//            glBindTexture(GL_TEXTURE_2D, texture);
//            glTexImage2D(
//                GL_TEXTURE_2D,
//                0,
//                GL_R8,
//                m_FontFace->glyph->bitmap.width,
//                m_FontFace->glyph->bitmap.rows,
//                0,
//                GL_RED,
//                GL_UNSIGNED_BYTE,
//                m_FontFace->glyph->bitmap.buffer
//            );
//            // set texture options
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            // now store character for later use
//            Character character = {
//                texture,
//                glm::ivec2(m_FontFace->glyph->bitmap.width, m_FontFace->glyph->bitmap.rows),
//                glm::ivec2(m_FontFace->glyph->bitmap_left, m_FontFace->glyph->bitmap_top),
//                m_FontFace->glyph->advance.x
//            };
//            m_Characters.insert(std::pair<char, Character>(c, character));
//        }
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
//	}
//
//	Font::~Font()
//	{
//		FT_Done_Face(m_FontFace);
//	}
//
//    SharedPtr<OpenGLTexture> Font::charToTexture(char c) const
//    {
//        return nullptr;
//    }
//
//    void Font::init()
//	{
//		RB_ASSERT(!FT_Init_FreeType(&s_Lib), "Freetype failed to initialize!");
//	}
//
//	void Font::deInit()
//	{
//		FT_Done_FreeType(s_Lib);
//	}
//
//}