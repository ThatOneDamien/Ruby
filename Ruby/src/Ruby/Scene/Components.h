#pragma once

#include <glm/glm.hpp>

namespace Ruby {

	namespace Components {

		struct Transform 
		{
			glm::vec2 Position{0.0f, 0.0f};
			float     Rotation{0.0f};
			glm::vec2    Scale{1.0f, 1.0f};

			Transform() = default;
			Transform(const Transform&) = default;
			Transform(const glm::vec2& pos, float rot, const glm::vec2& scale)
				: Position(pos), Rotation{ rot }, Scale(scale) {}
		};

		struct Sprite
		{
			glm::vec4 Color;

			Sprite() = default;
			Sprite(const Sprite&) = default;
			Sprite(const glm::vec4& color)
				: Color(color) {}
		};

	}

}