#pragma once

//#include "API/OpenGL/OpenGLTexture.h"
//
//#include <glm/glm.hpp>
//#include <unordered_map>
//
//struct FT_FaceRec_;
//typedef struct FT_FaceRec_* FT_Face;
//
//namespace Ruby {
//
//	struct Character 
//	{
//		uint32_t TextureID;      // ID handle of the glyph texture
//		glm::ivec2 Size;         // Size of glyph
//		glm::ivec2 Bearing;      // Offset from baseline to left/top of glyph
//		uint32_t Advance;        // Offset to advance to next glyph
//	};
//
//	class Font
//	{
//	public:
//		Font(const std::string& filepath, uint32_t pixelSize=12);
//		~Font();
//
//		inline void setPixelSize(uint32_t size) { m_PixelSize = size; }
//		inline uint32_t getPixelSize() const { return m_PixelSize; }
//
//		inline uint32_t getTextureID(char character) 
//		{ 
//			if (m_Characters.count(character)) 
//				return m_Characters[character].TextureID; 
//			return 0; 
//		}
//
//		SharedPtr<OpenGLTexture> charToTexture(char c) const;
//
//		static void init();
//		static void deInit();
//	private:
//		FT_Face m_FontFace;
//		uint32_t m_PixelSize;
//		std::unordered_map<char, Character> m_Characters;
//	};
//
//}