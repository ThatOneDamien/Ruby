#include "ruby_pch.h"

#define RB_USE_VULKAN
#define RB_USE_OPENGL
#define RB_ALL_GRAPHICS_APIS
#include "Context.h"

namespace Ruby
{
    namespace OpenGLContext { void init(); }
    namespace VulkanContext { void init(); void deinit(); }

    namespace Context
    {
        static API s_ContextAPI = API::None;
        static bool s_ContextInitialized = false;

        void initAPI(API desiredAPI)
        {
            if(s_ContextInitialized)
            {
                RB_WARN("Attempted to initialize rendering context multiple times. Ignoring.");
                return;
            }
            switch(desiredAPI)
            {
            case API::OpenGL:
                OpenGLContext::init();
                break;
            case API::Vulkan:
                // VulkanContext::init();
                break;
            default:
                RB_ERROR_DEBUG("Attempted to initialize unknown graphics API.");
                return;
            }
            s_ContextAPI = desiredAPI;
            s_ContextInitialized = true;
        }

        void deinitAPI()
        {
            if(!s_ContextInitialized)
            {
                RB_WARN("Attempted to uninitialize null context. Ignoring.");
                return;
            }
            switch(s_ContextAPI)
            {
            case API::Vulkan:
                // VulkanContext::deinit();
                break;
            default:
                RB_ERROR_DEBUG("Attempted to deinitialize unknown graphics API.");
                return;
            }
            s_ContextInitialized = false;
        }

        API getAPI()
        {
            return s_ContextAPI;
        }


        int getBindableTextureSlots()
        {
            switch(s_ContextAPI)
            {
            case API::OpenGL:
                return OpenGLContext::getBindableTextureSlots();
            case API::Vulkan:
                // return VulkanContext::getBindableTextureSlots();
            default:
                RB_ERROR_DEBUG("Unknown API.");
                return -1;
            }
        }

        void drawCall(const SharedPtr<VertexArray>& vao, uint32_t indexCount)
        {
            switch(s_ContextAPI)
            {
            case API::OpenGL:
                OpenGLContext::drawCall(vao, indexCount);
                break;
            case API::Vulkan:
                // VulkanContext::drawCall(vao, indexCount);
                break;
            default:
                RB_ERROR_DEBUG("Unknown API.");
                break;
            }
        }

        void setClearColor(float r, float g, float b)
        {
            switch(s_ContextAPI)
            {
            case API::OpenGL:
                OpenGLContext::setClearColor(r, g, b);
                break;
            case API::Vulkan:
                // VulkanContext::setClearColor(r, g, b);
                break;
            default:
                RB_ERROR_DEBUG("Unknown API.");
                break;
            }
        }

        void setClearColor(uint8_t r, uint8_t g, uint8_t b)
        {
            switch(s_ContextAPI)
            {
            case API::OpenGL:
                OpenGLContext::setClearColor(r, g, b);
                break;
            case API::Vulkan:
                // VulkanContext::setClearColor(r, g, b);
                break;
            default:
                RB_ERROR_DEBUG("Unknown API.");
                break;
            }
        }

        void setClearColor(const glm::vec3& color)
        {
            switch(s_ContextAPI)
            {
            case API::OpenGL:
                OpenGLContext::setClearColor(color);
                break;
            case API::Vulkan:
                // VulkanContext::setClearColor(color);
                break;
            default:
                RB_ERROR_DEBUG("Unknown API.");
                break;
            }
        }

        void setViewport(int x, int y, int width, int height)
        {
            switch(s_ContextAPI)
            {
            case API::OpenGL:
                OpenGLContext::setViewport(x, y, width, height);
                break;
            case API::Vulkan:
                // VulkanContext::setViewport(x, y, width, height);
                break;
            default:
                RB_ERROR_DEBUG("Unknown API.");
                break;
            }
        }

        void clear()
        {
            switch(s_ContextAPI)
            {
            case API::OpenGL:
                OpenGLContext::clear();
                break;
            case API::Vulkan:
                // VulkanContext::clear();
                break;
            default:
                RB_ERROR_DEBUG("Unknown API.");
                break;
            }
        }
    }
}
