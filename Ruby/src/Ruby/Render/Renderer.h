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

		void init();
		void deInit();

		void useCamera(const Camera& cam);
		void useFont(const SharedPtr<Font>& font);
		void renderSubmit(const SharedPtr<VertexArray>& vao, const SharedPtr<Shader> shader);


		// *** BATCH RENDERER ***
		

		void resetBatch();
		// Render the current batched geometry. Optional parameter cam
		// can be used to render based on a user defined camera rather
		// than the main camera of the currently bound scene. This functionality
		// is around to support applications that do not wish to use scenes.
		void renderBatch(const Camera* cam = nullptr);

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