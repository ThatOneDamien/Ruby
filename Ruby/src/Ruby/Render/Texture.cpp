#include "ruby_pch.h"

#include "Context.h"
#include "Texture.h"
#include "API/OpenGL/OpenGLTexture.h"
#include "Ruby/Main/Core.h"

namespace Ruby 
{
    SharedPtr<Texture> Texture::create(const std::string& filepath, const TextureSpec& spec)
    {
        switch(Context::getAPI())
        {
        case API::OpenGL:
            return createShared<OpenGLTexture>(filepath, spec);
        case API::Vulkan:
            // return createShared<VulkanVB>(vertices, size);
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
    }

    SharedPtr<Texture> Texture::create(const TextureSpec& spec)
    {
        switch(Context::getAPI())
        {
        case API::OpenGL:
            return createShared<OpenGLTexture>(spec);
        case API::Vulkan:
            // return createShared<VulkanVB>(vertices, size);
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
    }

    SharedPtr<Texture> Texture::create(const void* data, const TextureSpec& spec)
    {
        switch(Context::getAPI())
        {
        case API::OpenGL:
            return createShared<OpenGLTexture>(data, spec);
        case API::Vulkan:
            // return createShared<VulkanVB>(vertices, size);
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
    }

}
