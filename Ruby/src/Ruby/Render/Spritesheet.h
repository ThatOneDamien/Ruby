#pragma once

#include "Texture.h"

namespace Ruby {

	class Spritesheet
	{
	public:
		Spritesheet(uint32_t xTileSize, uint32_t yTileSize)
			: m_TileSizeX(xTileSize), m_TileSizeY(yTileSize)
		{
			glm::vec2 size = m_Texture->getSize();
			m_CountX = (uint32_t)floor(size.x / (float)xTileSize);
			m_CountY = (uint32_t)floor(size.y / (float)yTileSize);
		}

		

	private:
		SharedPtr<Texture> m_Texture;
		uint32_t m_TileSizeX, m_TileSizeY;
		uint32_t m_CountX, m_CountY;
		
	};

}