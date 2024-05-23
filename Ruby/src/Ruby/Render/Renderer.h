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
			void drawCall(const SharedPtr<VertexArray>& vao, uint32_t indexCount);
			void setClearColor(float r, float g, float b);
			void setClearColor(uint8_t r, uint8_t g, uint8_t b);
			void setClearColor(const glm::vec3& color);
			void setViewport(int x, int y, int width, int height);
			void clear();
			int getBindableTextureSlots();

		}

		void init();
		void deInit();
		
		void updateCamera(const Camera& cam);
		void renderSubmit(const SharedPtr<VertexArray>& vao, const SharedPtr<Shader> shader);


		// BATCH RENDERER
		void resetBatch();
		void renderBatch();

		void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

		void drawQuadTex(const glm::vec2& position, const glm::vec2& size,
						 const SharedPtr<Texture>& texture, const glm::vec4& color = glm::vec4{ 1.0f });

		void drawQuadSubTex(const glm::vec2& position, const glm::vec2& size, 
							const SharedPtr<SubTexture>& subTexture, const glm::vec4& color = glm::vec4{ 1.0f });

		void drawQuadRot(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		void drawQuadRotTex(const glm::vec2& position, const glm::vec2& size, float rotation, 
							const SharedPtr<Texture>& texture, const glm::vec4& color = glm::vec4{1.0f});

		void drawQuadRotSubTex(const glm::vec2& position, const glm::vec2& size, float rotation,
							   const SharedPtr<SubTexture>& subTexture, const glm::vec4& color = glm::vec4{ 1.0f });

		void drawText(const std::string& str, const glm::vec2& position, float size, float rotation, const glm::vec4& color);

	}

}