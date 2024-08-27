#include "ruby_pch.h"

#include "Buffers.h"
#include "Context.h"
#include "API/OpenGL/OpenGLBuffers.h"
#include "Ruby/Main/Core.h"


namespace Ruby 
{
    SharedPtr<VertexBuffer> VertexBuffer::createVBO(const void* vertices, uint32_t size)
    {
        switch(Context::getAPI())
        {
        case API::OpenGL:
            return createShared<OpenGLVB>(vertices, size);
        case API::Vulkan:
            // return createShared<VulkanVB>(vertices, size);
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
    }

    SharedPtr<VertexBuffer> VertexBuffer::createVBO(uint32_t size)
    {
        switch(Context::getAPI())
        {
        case API::OpenGL:
            return createShared<OpenGLVB>(size);
        case API::Vulkan:
            // return createShared<VulkanVB>(vertices, size);
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
    }

    SharedPtr<IndexBuffer> IndexBuffer::createIBO(const uint32_t* indices, uint32_t count)
    {
        switch(Context::getAPI())
        {
        case API::OpenGL:
            return createShared<OpenGLIB>(indices, count);
        case API::Vulkan:
            // return createShared<VulkanVB>(vertices, size);
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
    }

    SharedPtr<UniformBuffer> UniformBuffer::createUBO(const void* data, uint32_t size, uint8_t binding)
    {
        switch(Context::getAPI())
        {
        case API::OpenGL:
            return createShared<OpenGLUB>(data, size, binding);
        case API::Vulkan:
            // return createShared<VulkanVB>(vertices, size);
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
    }

    SharedPtr<UniformBuffer> UniformBuffer::createUBO(uint32_t size, uint8_t binding)
    {
        switch(Context::getAPI())
        {
        case API::OpenGL:
            return createShared<OpenGLUB>(size, binding);
        case API::Vulkan:
            // return createShared<VulkanVB>(vertices, size);
        default:
            RB_ERROR_DEBUG("Unknown or unsupported graphics API.");
            return nullptr;
        }
    }

}
