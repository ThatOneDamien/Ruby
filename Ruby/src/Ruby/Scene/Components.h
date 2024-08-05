#pragma once

#include <glm/glm.hpp>

#include "Ruby/Render/Texture.h"
#include "Ruby/Render/Camera.h"

namespace Ruby 
{
    namespace Components 
    {
        struct Transform
        {
            glm::vec3 Position{0.0f, 0.0f, 0.0f};
            float     Rotation{0.0f};
            glm::vec2    Scale{1.0f, 1.0f};

            Transform() = default;
            Transform(const Transform&) = default;
            Transform(const glm::vec3& pos, float rot, const glm::vec2& scale)
                : Position(pos), Rotation{ rot }, Scale(scale) {}
        };

        struct Sprite
        {
            SharedPtr<Texture> Tex{ nullptr };
            glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

            Sprite() = default;
            Sprite(const Sprite&) = default;
            Sprite(const SharedPtr<Texture>& tex, const glm::vec4& color)
                : Tex{ tex }, Color{ color } {}
        };

        struct Camera
        {
            Ruby::Camera Cam;
            // This will create a camera object with the aspect ratio of
            // the current window and scale of 1.0.
            Camera() = default;
            Camera(const Ruby::Camera& cam)
                : Cam(cam) {}

        };

    }

}
