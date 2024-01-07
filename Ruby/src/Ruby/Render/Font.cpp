#include "ruby_pch.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Font.h"
#include "Ruby/Main/App.h"

namespace Ruby {

	static FT_Library s_Lib{ nullptr };
	constexpr int Font::GLYPH_CNT;
	constexpr char Font::DESIRED_GLYPHS[95];

	Font::Font(const std::string& filepath, uint32_t pixelSize)
		: m_PixelSize(pixelSize)
	{
		RB_ASSERT(s_Lib, "FT Library not initialized, maybe you forgot to call Ruby::Font::init()?");


		RB_ASSERT(!FT_New_Face(s_Lib, filepath.c_str(), 0, &m_FontFace), "Unable to load font '%s'.", filepath.c_str());
		FT_Set_Pixel_Sizes(m_FontFace, 0, m_PixelSize);

		uint32_t textureW = 0, textureH = 0;
		const uint32_t SCREEN_WIDTH = App::getInstance().getWindow().getWidth();

		m_Metrics = { 
			(int16_t)(m_FontFace->ascender >> 6), 
			(int16_t)(m_FontFace->descender * 0.015625),
			(int16_t)(m_FontFace->height >> 6), 
			0 
		};

		getAtlasDims(&textureW, &textureH, SCREEN_WIDTH);
		generateAtlasTex(textureW, textureH, SCREEN_WIDTH);
	}

	Font::~Font()
	{
		if (s_Lib)
			FT_Done_Face(m_FontFace);
	}

	void Font::init()
	{
		RB_ASSERT(!FT_Init_FreeType(&s_Lib), "Freetype failed to initialize!");
	}

	void Font::deInit()
	{
		FT_Done_FreeType(s_Lib);
		s_Lib = nullptr;
	}

	void Font::getAtlasDims(uint32_t* width, uint32_t* height, uint32_t cutoff)
	{
		uint32_t curRowW = 0, curRowH = 0;
		uint32_t maxW = 0, maxH = 0;

		for (int i = 0; i < GLYPH_CNT; i++)
		{
			if (FT_Load_Char(m_FontFace, DESIRED_GLYPHS[i], FT_LOAD_RENDER))
			{
				RB_ERROR("Unable to load glyph %c", DESIRED_GLYPHS[i]);
				continue;
			}

			FT_Bitmap& bmp = m_FontFace->glyph->bitmap;

			if (bmp.width == 0 || bmp.rows == 0)
				continue;

			if (curRowW + bmp.width + 1 >= cutoff)
			{
				maxW = std::max(maxW, curRowW);
				maxH += curRowH;
				curRowW = 0;
				curRowH = 0;
			}
			curRowW += bmp.width + 1;
			curRowH = std::max(curRowH, bmp.rows);
		}

		*width = std::max(maxW, curRowW);
		*height = curRowH + maxH;
	}

	void Font::generateAtlasTex(uint32_t texW, uint32_t texH, uint32_t cutoff)
	{
		uint8_t* pixels = new uint8_t[(size_t)texW * texH];
		memset(pixels, 0, (size_t)texW * texH);

		RB_ASSERT(pixels && texW * texH > 1, "'pixels' was nullptr, possibly ran out of memory.");

		TextureSpec spec;
		spec.Format = PixelFormat::R8;
		spec.Width = texW;
		spec.Height = texH;
		spec.MagFilter = TextureFilter::Linear;
		spec.MinFilter = TextureFilter::Linear;
		spec.WrapS = TextureWrap::ClampToEdge;
		spec.WrapT = TextureWrap::ClampToEdge;

		m_AtlasTexture = Texture::createTexture(spec);


		uint32_t offX = 0, offY = 0;
		uint32_t curRowH = 0;

		double invW = 1.0 / texW;
		double invH = 1.0 / texH;

		if (FT_Load_Char(m_FontFace, ' ', FT_LOAD_RENDER))
		{
			RB_ERROR("Unable to load space key glyph.");
		}
		else
		{
			m_Metrics.SpaceSize = m_FontFace->glyph->advance.x >> 6;
		}

		for (int i = 0; i < GLYPH_CNT; i++)
		{
			uint8_t ch = DESIRED_GLYPHS[i];

			// load character glyph 
			if (FT_Load_Char(m_FontFace, ch, FT_LOAD_RENDER))
			{
				RB_ERROR("Unable to load glyph %c", ch);
				continue;
			}

			FT_Bitmap& bmp = m_FontFace->glyph->bitmap;

			if (bmp.width == 0 || bmp.rows == 0)
				continue;

			if (offX + bmp.width + 1 >= cutoff)
			{
				offY += curRowH;
				curRowH = 0;
				offX = 0;
			}

			for (int row = 0; row < bmp.rows; row++)
			{
				for (int col = 0; col < bmp.width; col++)
				{
					pixels[(offY + row) * texW + offX + col] = bmp.buffer[row * bmp.width + col];
				}
			}

			SharedPtr<SubTexture> sbt = SubTexture::createSubTexture(
				m_AtlasTexture,
				{ (double)offX * invW, (double)(offY + bmp.rows) * invH },
				{ (double)(offX + bmp.width) * invW, (double)offY * invH }
			);


			// now store character for later use
			CharMetrics character = {
				sbt,
				glm::ivec2(bmp.width, bmp.rows),
				glm::ivec2(m_FontFace->glyph->bitmap_left, m_FontFace->glyph->bitmap_top),
				m_FontFace->glyph->advance.x >> 6
			};

			m_Characters.emplace(ch, character);

			curRowH = std::max(curRowH, bmp.rows);
			offX += bmp.width + 1;
		}

		m_AtlasTexture->setData(pixels, texW * texH);

		delete[] pixels;
	}

	const FontMetrics& Font::getMetrics() const 
	{
		return m_Metrics;
	}
}