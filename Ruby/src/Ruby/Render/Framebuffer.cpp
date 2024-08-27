#include "ruby_pch.h"

#include "Context.h"
#include "Framebuffer.h"
#include "API/OpenGL/OpenGLFramebuffer.h"
#include "Ruby/Main/Core.h"

namespace Ruby 
{
    SharedPtr<Framebuffer> Framebuffer::create(int width, int height)
    {
        switch(Context::getAPI())
        {
        case API::OpenGL:
            return createShared<OpenGLFramebuffer>(width, height);
        case API::Vulkan:
            // return createShared<VulkanVB>(vertices, size);
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
    }

}
