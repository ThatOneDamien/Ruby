// #include "API/OpenGL/OpenGLShader.h"
#include "ruby_pch.h"

#include "Context.h"
#include "Shader.h"
#include "Ruby/Main/Core.h"
#include "API/OpenGL/OpenGLShader.h" 

#include <cstring>


namespace Ruby 
{
    SharedPtr<Shader> Shader::create(const ProgramStages& stageSrcs, const ProgramStages& stageCaches)
    {
        switch(Context::getAPI())
        {
        case API::OpenGL:
            return createShared<OpenGLShader>(stageSrcs, stageCaches);
        case API::Vulkan:
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
    }


}
