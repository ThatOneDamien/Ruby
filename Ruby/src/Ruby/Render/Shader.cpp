#include "ruby_pch.h"

#include "Context.h"
#include "Shader.h"
#include "Ruby/Main/Core.h"
#include "API/OpenGL/Shader.h" 

#include <cstring>


namespace Ruby 
{

    static const char* SHADER_TYPE_TO_STR[6] = 
    { 
        "Vertex", 
        "Tesselation Control", 
        "Tesselation Evaluation",
        "Geometry",
        "Fragment",
        "Compute"
    };

    const char* shaderTypeToString(ShaderType type)
    {
        return SHADER_TYPE_TO_STR[(size_t)type];
    }

    // SharedPtr<Shader> Shader::create(const ProgramStages& stagePaths, const ProgramStages& cachePaths)
    // {
    //     switch(Context::getAPI())
    //     {
    //     case API::OpenGL:
    //         return createShared<OpenGLShader>(stagePaths, cachePaths);
    //     case API::Vulkan:
    //     default:
    //         RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
    //         return nullptr;
    //     }
    // }

    SharedPtr<Shader> Shader::createCombined(const std::string& filepath, bool shouldCache, const std::string& cachePath)
    {
        switch(Context::getAPI())
        {
        case API::OpenGL:
            return createShared<OpenGLShader>(filepath, shouldCache, cachePath);
        case API::Vulkan:
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
    }

}
