#pragma once

#include "API/OpenGL/OpenGLTexture.h"

#include <glm/glm.hpp>
#include <unordered_map>

struct FT_FaceRec_;
typedef struct FT_FaceRec_* FT_Face;

namespace Ruby {

	struct FontMetrics
	{
		int16_t Ascender;
		int16_t Descender;
		int16_t Height;
		int64_t SpaceSize;
	};

	struct CharMetrics
	{
		SharedPtr<SubTexture> SubTex{ nullptr }; // Texture ref
		glm::ivec2            Size{ 0, 0 };      // Size of glyph
		glm::ivec2            Bearing{ 0, 0 };   // Offset from baseline to left/top of glyph
		uint32_t              Advance{ 0 };      // Offset to advance to next glyph
	};

	class Font
	{
	public:
		Font(const std::string& filepath, uint32_t pixelSize=12);
		~Font();

		inline void setPixelSize(uint32_t size) { m_PixelSize = size; }
		inline uint32_t getPixelSize() const { return m_PixelSize; }

		inline const SharedPtr<SubTexture>& getCharSubTexture(char ch)
		{
			if (m_Characters.count(ch))
				return m_Characters[ch].SubTex;
			return nullptr;
		}

		inline const CharMetrics& getCharMetrics(char ch)
		{
			if (m_Characters.count(ch))
				return m_Characters[ch];
			return {};
		}

		inline const SharedPtr<Texture>& getAtlasTexture()
		{
			return m_AtlasTexture;
		}

		const FontMetrics& getMetrics() const;

		static void init();
		static void deInit();
	private:
		void getAtlasDims(uint32_t* width, uint32_t* height, uint32_t cutoff);
		void generateAtlasTex(uint32_t texW, uint32_t texH, uint32_t cutoff);

	private:
		SharedPtr<Texture> m_AtlasTexture;
		FontMetrics m_Metrics;
		FT_Face m_FontFace;
		uint32_t m_PixelSize;
		std::unordered_map<char, CharMetrics> m_Characters;
		// The characters we desire to get from the font face.
		static constexpr char DESIRED_GLYPHS[] =
			"!\"#$%&'()*+,-./0123456789:;<=>?@"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
			"abcdefghijklmnopqrstuvwxyz{|}~";

		static constexpr int GLYPH_CNT = sizeof(DESIRED_GLYPHS);
	};

}