#include "ruby_pch.h"

#include "Context.h"
#include "VertexArray.h"
#include "API/OpenGL/OpenGLVertexArray.h"
#include "Ruby/Main/Core.h"

namespace Ruby 
{
    SharedPtr<VertexArray> VertexArray::createVAO()
    {
        switch(Context::getAPI())
        {
        case API::OpenGL:
            return createShared<OpenGLVA>();
        case API::Vulkan:
            // return createShared<VulkanVB>(vertices, size);
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
    }

}
