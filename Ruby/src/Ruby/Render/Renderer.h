#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

namespace Ruby {

	namespace Renderer {

		void init();
		void deInit();

		void renderIndices(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Shader> shader, const glm::mat4& transform);
		void resetBatch();
		void renderBatched();
		void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		void drawTexture(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture);

		// GL CONTEXT STUFF
		void setClearColor(float r, float g, float b, float a);
		void setClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void setClearColor(const glm::vec4& color);
		void setViewport(int x, int y, int width, int height);
		void clear();

	}

}