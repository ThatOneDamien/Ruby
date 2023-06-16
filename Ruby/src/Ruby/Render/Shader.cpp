#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Shader.h"

#ifdef RB_USE_OPENGL
#include "API/OpenGL/OpenGLShader.h"
#define SHADER OpenGLShader
#endif

namespace Ruby {

	std::unordered_map<std::string, std::shared_ptr<Shader>> Shader::s_ShaderLibrary;

	std::shared_ptr<Shader> Shader::createShader(const std::string& filepath)
	{
		return std::make_shared<SHADER>(filepath);
	}

	std::shared_ptr<Shader> Shader::createShader(const std::string& name, const std::string& filepath)
	{
		return std::make_shared<SHADER>(name, filepath);
	}

	std::shared_ptr<Shader> Shader::createShader(const std::string& name, const std::string& vertSrc, const std::string& fragSrc)
	{
		return std::make_shared<SHADER>(name, vertSrc, fragSrc);
	}

	std::shared_ptr<Shader> Shader::createAndAddShaderToLibrary(const std::string& filepath)
	{
		auto shader = std::make_shared<SHADER>(filepath);
		s_ShaderLibrary[shader->getName()] = shader;
		return shader;
	}

	std::shared_ptr<Shader> Shader::createAndAddShaderToLibrary(const std::string& name, const std::string& filepath)
	{
		auto shader = std::make_shared<SHADER>(name, filepath);
		s_ShaderLibrary[name] = shader;
		return shader;
	}

	std::shared_ptr<Shader> Shader::createAndAddShaderToLibrary(const std::string& name, const std::string& vertSrc, const std::string& fragSrc)
	{
		auto shader = std::make_shared<SHADER>(name, vertSrc, fragSrc);
		s_ShaderLibrary[name] = shader;
		return shader;
	}

}