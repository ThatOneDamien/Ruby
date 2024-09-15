#pragma once

#include "Ruby/Main/Core.h"
#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Ruby
{

    enum class API
    {
        None = 0,
        OpenGL,
        Vulkan
    };

    // May add the rest later
    enum class Primitive
    {
        Points    = 0,
        Lines     = 1,
        Triangles = 4
    };

    namespace Context
    {
        void initAPI(API desiredAPI);
        void deinitAPI();
        API getAPI();
        int getBindableTextureSlots();
        void drawCall(const SharedPtr<VertexArray>& vao, 
                      uint32_t indexCount = 0, 
                      Primitive primtive = Primitive::Triangles);

        void drawInstanced(const SharedPtr<VertexArray>& vao, 
                           uint32_t instanceCount = 1, 
                           uint32_t indexCount = 0, 
                           Primitive primtive = Primitive::Triangles);

        void setClearColor(float r, float g, float b);
        void setClearColor(uint8_t r, uint8_t g, uint8_t b);
        void setClearColor(const glm::vec3& color);
        void setViewport(int x, int y, int width, int height);
        void clear();
    }
}
