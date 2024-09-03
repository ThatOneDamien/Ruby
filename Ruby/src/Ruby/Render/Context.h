#pragma once

#include "Ruby/Main/Core.h"
#include "VertexArray.h"

#include <glm/glm.hpp>

// Used to only include necessary functions for 
// desired API so as to prevent loading functions for other APIs
// into the final executable.
#if defined(RB_USE_OPENGL)
#undef RB_USE_VULKAN
#include "API/OpenGL/OpenGLContext.h"
#elif defined(RB_USE_VULKAN)
#include "API/Vulkan/VulkanContext.h"
#else
#define RB_ALL_GRAPHICS_APIS
#endif

namespace Ruby
{

    enum class API
    {
        None = 0,
        OpenGL,
        Vulkan
    };

#ifdef RB_ALL_GRAPHICS_APIS 

    namespace Context
    {
        void initAPI(API desiredAPI);
        void deinitAPI();
        API getAPI();
        int getBindableTextureSlots();
        void drawCall(const SharedPtr<VertexArray>& vao, uint32_t indexCount = 0);
        void drawInstanced(const SharedPtr<VertexArray>& vao, uint32_t instanceCount = 1, uint32_t indexCount = 0);
        void setClearColor(float r, float g, float b);
        void setClearColor(uint8_t r, uint8_t g, uint8_t b);
        void setClearColor(const glm::vec3& color);
        void setViewport(int x, int y, int width, int height);
        void clear();
    }
#endif
}
