#pragma once

#include <glm/glm.hpp>

namespace Ruby {

	namespace Renderer {

		void init();
		void setClearColor(float r, float g, float b, float a);
		void setClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void setClearColor(const glm::vec4& color);
		void clear();
	}

}