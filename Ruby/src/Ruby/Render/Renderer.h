#pragma once

#include "VertexArray.h"
#include "Buffers.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"


#include <glm/glm.hpp>

namespace Ruby {

	namespace Renderer {

		// API INTRINSIC
		namespace API {

			void initAPI();
			void deInitAPI();
			void drawCall(const SharedPtr<VertexArray>& vao, uint32_t indexCount = 0);
			void setClearColor(float r, float g, float b, float a);
			void setClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
			void setClearColor(const glm::vec4& color);
			void setViewport(int x, int y, int width, int height);
			void clear();

		}

		void init();
		void deInit();
		
		void updateCamera(const Camera& cam);
		void renderSubmit(const SharedPtr<VertexArray>& vao, const SharedPtr<Shader> shader, const glm::mat4& transform);


		// BATCH RENDERER
		void resetBatch();
		void renderBatched();
		void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		void drawQuadTexture(const glm::vec2& position, const glm::vec2& size, const SharedPtr<Texture>& texture);
		void drawQuadSubTexture(const glm::vec2& position, const glm::vec2& size, const SharedPtr<SubTexture>& subTexture);
		void drawQuadRotated(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

	}

}