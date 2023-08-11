#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Texture.h"

#ifdef RB_USE_OPENGL
#include "API/OpenGL/OpenGLTexture.h"
#define TEXTURE OpenGLTexture
#endif
namespace Ruby {

	SharedPtr<Texture> Texture::createTexture(const std::string& filepath)
	{
		return createShared<TEXTURE>(filepath);
	}

	SharedPtr<Texture> Texture::createTexture(const TextureSpec& spec)
	{
		return createShared<TEXTURE>(spec);
	}

	SharedPtr<Texture> Texture::createTexture(const void* data, const TextureSpec& spec)
	{
		return createShared<TEXTURE>(data, spec);
	}

	SubTexture::SubTexture(const SharedPtr<Texture>& texture, const glm::vec2& bottomLeftCorner, const glm::vec2& topRightCorner)
	{
		m_Texture = texture;
		m_TexCoords[0] = { bottomLeftCorner.x, bottomLeftCorner.y };
		m_TexCoords[1] = { topRightCorner.x, bottomLeftCorner.y };
		m_TexCoords[2] = { topRightCorner.x, topRightCorner.y };
		m_TexCoords[3] = { bottomLeftCorner.x, topRightCorner.y };
	}

}