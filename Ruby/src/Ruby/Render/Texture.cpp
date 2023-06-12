#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Texture.h"

#ifdef RB_USE_OPENGL
#include "API/OpenGL/OpenGLTexture.h"
#define TEXTURE OpenGLTexture
#endif
namespace Ruby {

	std::shared_ptr<Texture> Texture::createTexture(const std::string& filepath)
	{
		return std::make_shared<TEXTURE>(filepath);
	}

	std::shared_ptr<Texture> Texture::createTexture(int width, int height)
	{
		return std::make_shared<TEXTURE>(width, height);
	}

}