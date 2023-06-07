#pragma once

#include <glm/glm.hpp>
#include "Ruby/Render/VertexArray.h"
#include "Ruby/Render/Shader.h"

namespace Ruby {

	namespace Renderer {

		void init();
		void setClearColor(float r, float g, float b, float a);
		void setClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void setClearColor(const glm::vec4& color);
		void setViewport(int x, int y, int width, int height);
		void clear();
		void renderIndices(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Shader> shader, const glm::mat4& transform);

	}

}