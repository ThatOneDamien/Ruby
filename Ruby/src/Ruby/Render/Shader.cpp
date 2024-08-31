#include "API/OpenGL/OpenGLShader.h"
#include "ruby_pch.h"

#include "Context.h"
#include "Shader.h"
#include "Ruby/Main/Core.h"


namespace Ruby 
{
    std::unordered_map<std::string, SharedPtr<Shader>> Shader::s_ShaderLibrary;

    SharedPtr<Shader> Shader::create(const std::string& filepath)
    {
        switch(Context::getAPI())
        {
        case API::OpenGL:
            return createShared<OpenGLShader>(filepath);
        case API::Vulkan:
            // return createShared<VulkanVB>(vertices, size);
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
    }

    SharedPtr<Shader> Shader::create(const std::string& name, const std::string& filepath)
    {
        switch(Context::getAPI())
        {
        case API::OpenGL:
            return createShared<OpenGLShader>(name, filepath);
        case API::Vulkan:
            // return createShared<VulkanVB>(vertices, size);
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
    }

    SharedPtr<Shader> Shader::createAndAddToLibrary(const std::string& filepath)
    {
        SharedPtr<Shader> shader;
        switch(Context::getAPI())
        {
        case API::OpenGL:
            shader = createShared<OpenGLShader>(filepath);
        case API::Vulkan:
            // return createShared<VulkanVB>(vertices, size);
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
        s_ShaderLibrary[shader->getName()] = shader;
        return shader;
    }

    SharedPtr<Shader> Shader::createAndAddToLibrary(const std::string& name, const std::string& filepath)
    {
        SharedPtr<Shader> shader;
        switch(Context::getAPI())
        {
        case API::OpenGL:
            shader = createShared<OpenGLShader>(name, filepath);
        case API::Vulkan:
            // return createShared<VulkanVB>(vertices, size);
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
        s_ShaderLibrary[name] = shader;
        return shader;
    }

}
