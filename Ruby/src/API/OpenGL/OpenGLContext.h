#pragma once

#include "Ruby/Main/Core.h"
#include "Ruby/Render/VertexArray.h"

#include <glm/glm.hpp>

namespace Ruby
{
    namespace OpenGLContext
    {
        int getBindableTextureSlots();
        void drawCall(const SharedPtr<VertexArray>& vao, uint32_t indexCount = 0);
        void drawInstanced(const SharedPtr<VertexArray>& vao, uint32_t instanceCount = 1, uint32_t indexCount = 0);
        void setClearColor(float r, float g, float b);
        void setClearColor(uint8_t r, uint8_t g, uint8_t b);
        void setClearColor(const glm::vec3& color);
        void setViewport(int x, int y, int width, int height);
        void clear();
    }
}
