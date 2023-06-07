#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Shader.h"

#ifdef RB_USE_OPENGL
#include "API/OpenGL/OpenGLShader.h"
#define SHADER OpenGLShader
#endif

namespace Ruby {

	std::shared_ptr<Shader> Shader::create(const std::string& filepath)
	{
		return std::make_shared<SHADER>(filepath);
	}

	std::shared_ptr<Shader> Shader::create(const std::string& name, const std::string& vertSrc, const std::string& fragSrc)
	{
		return std::make_shared<SHADER>(name, vertSrc, fragSrc);
	}

}