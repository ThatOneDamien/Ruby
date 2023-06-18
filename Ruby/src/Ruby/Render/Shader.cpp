#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Shader.h"

#ifdef RB_USE_OPENGL
#include "API/OpenGL/OpenGLShader.h"
#define SHADER OpenGLShader
#endif

namespace Ruby {

	std::unordered_map<std::string, SharedPtr<Shader>> Shader::s_ShaderLibrary;

	SharedPtr<Shader> Shader::createShader(const std::string& filepath)
	{
		return createShared<SHADER>(filepath);
	}

	SharedPtr<Shader> Shader::createShader(const std::string& name, const std::string& filepath)
	{
		return createShared<SHADER>(name, filepath);
	}

	SharedPtr<Shader> Shader::createShader(const std::string& name, const std::string& vertSrc, const std::string& fragSrc)
	{
		return createShared<SHADER>(name, vertSrc, fragSrc);
	}

	SharedPtr<Shader> Shader::createAndAddShaderToLibrary(const std::string& filepath)
	{
		auto shader = createShared<SHADER>(filepath);
		s_ShaderLibrary[shader->getName()] = shader;
		return shader;
	}

	SharedPtr<Shader> Shader::createAndAddShaderToLibrary(const std::string& name, const std::string& filepath)
	{
		auto shader = createShared<SHADER>(name, filepath);
		s_ShaderLibrary[name] = shader;
		return shader;
	}

	SharedPtr<Shader> Shader::createAndAddShaderToLibrary(const std::string& name, const std::string& vertSrc, const std::string& fragSrc)
	{
		auto shader = createShared<SHADER>(name, vertSrc, fragSrc);
		s_ShaderLibrary[name] = shader;
		return shader;
	}

}