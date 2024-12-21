#include "ruby_pch.h"

#include "Context.h"
#include "Cubemap.h"

#include "API/OpenGL/Cubemap.h"

namespace Ruby
{
    SharedPtr<Cubemap> Cubemap::create(const CubemapTexPaths& paths)
    {
        switch(Ruby::Context::getAPI())
        {
        case Ruby::API::OpenGL:
            return createShared<OpenGLCubemap>(paths);
        case Ruby::API::Vulkan:
            // return createShared<>
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;

        }
    }
}
