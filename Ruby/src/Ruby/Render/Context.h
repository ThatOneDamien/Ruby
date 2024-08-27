#pragma once

#include <glm/glm.hpp>

#include "Ruby/Main/Core.h"
#include "VertexArray.h"

// Used to only include necessary functions for 
// desired API so as to prevent loading functions for other APIs
// into the final executable.
#if defined(RB_USE_OPENGL) && defined(RB_USE_VULKAN)
#undef RB_USE_VULKAN
#elif !defined(RB_USE_OPENGL) && !defined(RB_USE_VULKAN)
#define RB_ALL_GRAPHICS_APIS
#endif

namespace Ruby
{
#ifdef RB_USE_OPENGL
    namespace OpenGLContext
    {
        int getBindableTextureSlots();
        void drawCall(const SharedPtr<VertexArray>& vao, uint32_t indexCount = 0);
        void setClearColor(float r, float g, float b);
        void setClearColor(uint8_t r, uint8_t g, uint8_t b);
        void setClearColor(const glm::vec3& color);
        void setViewport(int x, int y, int width, int height);
        void clear();
    }
#endif

#ifdef RB_USE_VULKAN
    namespace VulkanContext
    {
        int getBindableTextureSlots();
        void drawCall(const SharedPtr<VertexArray>& vao, uint32_t indexCount = 0);
        void setClearColor(float r, float g, float b);
        void setClearColor(uint8_t r, uint8_t g, uint8_t b);
        void setClearColor(const glm::vec3& color);
        void setViewport(int x, int y, int width, int height);
        void clear();
    }
#endif

#ifdef RB_ALL_GRAPHICS_APIS 
    enum class API
    {
        None = 0,
        OpenGL,
        Vulkan
    };

    namespace Context
    {
        void initAPI(API desiredAPI);
        void deinitAPI();
        API getAPI();
        int getBindableTextureSlots();
        void drawCall(const SharedPtr<VertexArray>& vao, uint32_t indexCount = 0);
        void setClearColor(float r, float g, float b);
        void setClearColor(uint8_t r, uint8_t g, uint8_t b);
        void setClearColor(const glm::vec3& color);
        void setViewport(int x, int y, int width, int height);
        void clear();
    }
#endif
}
