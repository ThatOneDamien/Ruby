#pragma once

#include "VertexArray.h"
#include "Buffers.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Ruby/Scene/Scene.h"
#include "Font.h"


#include <glm/glm.hpp>

namespace Ruby 
{

    namespace Renderer 
    {
        // *** API INTRINSIC ***
        namespace API 
        {
            void drawCall(const SharedPtr<VertexArray>& vao, uint32_t indexCount = 0);
            void setClearColor(float r, float g, float b);
            void setClearColor(uint8_t r, uint8_t g, uint8_t b);
            void setClearColor(const glm::vec3& color);
            void setViewport(int x, int y, int width, int height);
            void clear();
        }

        // Clears any previous data, allocates new storage, and initializes 
        // the 2D renderer to its base state.
        void init();

        // Deallocates all data owned by the renderer, and
        // sets it to a blank state. By calling this manually,
        // you effectively remove batch rendering capabilities.
        void clear();


        // Use provided Camera's projection matrix in the GPU for
        // projection of the batched geometry.
        void useCamera(const Camera& cam);
        // Use font provided for text rendering.
        void useFont(const SharedPtr<Font>& font);

        // *** BATCH RENDERER ***
        
        // Reset the batch renderer to a blank state with no vertex data.
        // You should call this every frame when rendering dynamic scenes
        // (so like, all the time).
        void resetBatch();
        // Render the current batched geometry. Optional parameter cam
        // can be used to render based on a user defined camera rather
        // than the main camera of the currently bound scene. This functionality
        // is around to support applications that do not wish to use scenes.
        void renderBatch(const Camera* cam = nullptr);

        // Draw an unrotated, untextured quad with variable size and color. Must be called within resetBatch and renderBatch.
        void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        // Draw an unrotated, textured quad with variable size and color.
        void drawQuadTex(const glm::vec2& position, const glm::vec2& size,
                         const SharedPtr<Texture>& texture, const glm::vec4& color = glm::vec4{ 1.0f });
        // Draw an unrotated, textured quad using the specific texture coordinates specified by
        // subTexture, with variable size and color. Must be called within resetBatch and renderBatch.
        void drawQuadSubTex(const glm::vec2& position, const glm::vec2& size, 
                            const SharedPtr<SubTexture>& subTexture, const glm::vec4& color = glm::vec4{ 1.0f });
        // Draw a rotated, untextured quad with variable size and color. Must be called within resetBatch and renderBatch.
        void drawQuadRot(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        // Draw a rotated, textured quad with variable size and color. Must be called within resetBatch and renderBatch.
        void drawQuadRotTex(const glm::vec2& position, const glm::vec2& size, float rotation, 
                            const SharedPtr<Texture>& texture, const glm::vec4& color = glm::vec4{1.0f});
        // Draw a rotated, textured quad using the specific texture coordinates specified by
        // subTexture, with variable size and color. Must be called within resetBatch and renderBatch.
        void drawQuadRotSubTex(const glm::vec2& position, const glm::vec2& size, float rotation,
                               const SharedPtr<SubTexture>& subTexture, const glm::vec4& color = glm::vec4{ 1.0f });
        // Draw the specified text with the position, scale, rotation, and color specified. 
        // Must be called within resetBatch and renderBatch.
        void drawText(const std::string& str, const glm::vec2& position, float scale, float rotation, const glm::vec4& color);

    }

}
