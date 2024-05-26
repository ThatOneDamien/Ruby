#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Texture.h"

#ifdef RB_USE_OPENGL
#include "API/OpenGL/OpenGLTexture.h"
#define TEXTURE OpenGLTexture
#endif
namespace Ruby 
{
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

}