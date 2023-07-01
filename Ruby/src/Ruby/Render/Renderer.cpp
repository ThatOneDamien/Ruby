#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Ruby/Render/Renderer.h"
#include "Ruby/Render/Font.h"
#include "Ruby/Main/App.h"

#pragma warning(disable: 6385, 6386) // Compiler warning about overflowing buffer which wont happen.

namespace Ruby {

	namespace Renderer
	{

		//
		//
		// -----------------BATCH RENDERER STUFF-------------------
		//
		//

		// 40 bytes in size, composed of position, color, texture coordinates, and texture slot.
		struct QuadVertex
		{
			union
			{
				struct { float x, y, z; };
				glm::vec3 position;
			};

			glm::vec4 color;
			glm::vec2 texCoords;
			float textureSlot;

		};

		static constexpr uint16_t MAX_QUADS = 250;
		static constexpr size_t MAX_VERTICES = MAX_QUADS * 4;
		static constexpr size_t MAX_INDICES = MAX_QUADS * 6;

		static SharedPtr<VertexArray> s_QuadVAO{ nullptr };
		static SharedPtr<VertexBuffer> s_QuadVBO{ nullptr };
		static SharedPtr<IndexBuffer> s_QuadIBO{ nullptr };
		static SharedPtr<Shader> s_QuadShader{ nullptr };

		static QuadVertex* s_QuadVBData{ nullptr };
		static QuadVertex* s_QuadVBOffset{ nullptr };
		static uint16_t s_QuadCount = 0;

		// TODO : Abstract the size of array into a system that auto detects the max number of texture slots for the card.
		// for now it is 31, with one of the slots being reserved for basic white 1 pixel texture for colored quads.
		static SharedPtr<Texture> s_BoundTextures[31] = { nullptr };
		static SharedPtr<Texture> s_BlankColorTexture{ nullptr };
		static uint8_t s_TextureInsert = 0;

		static glm::mat4 s_CameraViewProj;
		static SharedPtr<UniformBuffer> s_CamUBO{ nullptr };

		//static Font* s_Font{ nullptr };

		// ---------------END BATCH RENDERER STUFF-----------------


		void init()
		{
			API::initAPI();

			s_QuadVAO = VertexArray::createVAO();
			s_QuadVBO = VertexBuffer::createVBO(MAX_VERTICES * sizeof(QuadVertex));

			{
				VertexLayout layout;
				layout.push({ LayoutType::Float, 3, false });
				layout.push({ LayoutType::Float, 4, false });
				layout.push({ LayoutType::Float, 2, false });
				layout.push({ LayoutType::Float, 1, false });

				s_QuadVBO->setLayout(layout);
			}

			s_QuadVAO->setVertexBuffer(s_QuadVBO);


			s_QuadVBData = new QuadVertex[MAX_VERTICES];

			{
				uint32_t indices[MAX_INDICES];

				// Pattern for indices 0, 1, 2, 2, 3, 0 for drawing a basic quad. This will always be the same no matter what so it
				// is better to only send the data to the graphics card once and not update it ass quads are added.
				uint32_t offset = 0;
				for (size_t i = 0; i < MAX_INDICES; i += 6, offset += 4)
				{
					indices[i] = offset;
					indices[i + 1] = offset + 1;
					indices[i + 2] = offset + 2;

					indices[i + 3] = offset + 2;
					indices[i + 4] = offset + 3;
					indices[i + 5] = offset;
				}

				s_QuadIBO = IndexBuffer::createIBO(indices, MAX_INDICES);
			}

			s_QuadVAO->setIndexBuffer(s_QuadIBO);

			s_BlankColorTexture = Texture::createTexture(1, 1);

			// This sets the data of the white texture to 1 pixel of white, which allows the card to multiply by color to produce
			// purely colored quads with no texture, while still allowing batching.
			uint32_t white = 0xFFFFFFFF;
			s_BlankColorTexture->setData((const void*)&white, sizeof(uint32_t));

			s_QuadShader = Shader::createShader("res/shaders/quad_shader.glsl");

			s_QuadShader->bind();
			s_CamUBO = UniformBuffer::createUBO(sizeof(glm::mat4), 0);
			{
				int arr[32];
				for (int i = 0; i < 32; i++)
					arr[i] = i;

				s_QuadShader->setUniformIntArray("u_Textures", 32, arr);
			}

			//s_Font = new Font("res/fonts/Roboto-Regular.ttf", 12);
		}



		void deInit()
		{
			//delete s_Font;
			delete[] s_QuadVBData;
			API::deInitAPI();
		}

		void updateCamera(const Camera& cam)
		{
			s_CameraViewProj = cam.getViewProjectionMatrix();
			s_CamUBO->setData(&s_CameraViewProj[0][0], sizeof(glm::mat4), 0);
		}

		void renderSubmit(const SharedPtr<VertexArray>& vao, const SharedPtr<Shader> shader, const glm::mat4& transform)
		{
			shader->bind();
			shader->setUniformMat4("u_ViewProj", transform);

			API::drawCall(vao);
		}



		void resetBatch()
		{
			s_QuadVBOffset = s_QuadVBData;
		}



		void renderBatched()
		{
			s_QuadVAO->bind();
			s_QuadVBO->setVertexData(s_QuadVBData, (uint32_t)((uint8_t*)s_QuadVBOffset - (uint8_t*)s_QuadVBData), 0);
			s_QuadShader->bind();

			s_BlankColorTexture->bind(0);
			for (int i = 0; i < 31 && s_BoundTextures[i]; i++)
				s_BoundTextures[i]->bind(i + 1);

			API::drawCall(s_QuadVAO);

			s_QuadCount = 0;
			s_TextureInsert = 0;

		}



		void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
		{
			if (s_QuadCount >= MAX_QUADS)
			{
				renderBatched();
				resetBatch();
			}
			float halfSize[2] = { size.x * 0.5f, size.y * 0.5f };
			s_QuadVBOffset[0].x = position.x - halfSize[0];
			s_QuadVBOffset[0].y = position.y - halfSize[1];
			s_QuadVBOffset[0].z = 0;
			s_QuadVBOffset[0].color = color;
			s_QuadVBOffset[0].texCoords = { 0.0f, 0.0f };
			s_QuadVBOffset[0].textureSlot = 0.0f;

			s_QuadVBOffset[1].x = position.x + halfSize[0];
			s_QuadVBOffset[1].y = position.y - halfSize[1];
			s_QuadVBOffset[1].z = 0;
			s_QuadVBOffset[1].color = color;
			s_QuadVBOffset[1].texCoords = { 1.0f, 0.0f };
			s_QuadVBOffset[1].textureSlot = 0.0f;

			s_QuadVBOffset[2].x = position.x + halfSize[0];
			s_QuadVBOffset[2].y = position.y + halfSize[1];
			s_QuadVBOffset[2].z = 0;
			s_QuadVBOffset[2].color = color;
			s_QuadVBOffset[2].texCoords = { 1.0f, 1.0f };
			s_QuadVBOffset[2].textureSlot = 0.0f;

			s_QuadVBOffset[3].x = position.x - halfSize[0];
			s_QuadVBOffset[3].y = position.y + halfSize[1];
			s_QuadVBOffset[3].z = 0;
			s_QuadVBOffset[3].color = color;
			s_QuadVBOffset[3].texCoords = { 0.0f, 1.0f };
			s_QuadVBOffset[3].textureSlot = 0.0f;

			s_QuadVBOffset += 4;
			s_QuadCount++;
		}



		void drawQuadTexture(const glm::vec2& position, const glm::vec2& size, const SharedPtr<Texture>& texture)
		{
			if (s_QuadCount >= MAX_QUADS || s_TextureInsert > 30)
			{
				renderBatched();
				resetBatch();
			}

			float textureSlot = 0.0f;
			for (int i = 0; i < 31; i++)
			{
				if (s_BoundTextures[i] == texture)
				{
					textureSlot = (float)i;
					break;
				}
			}

			if (textureSlot == 0.0f)
			{
				textureSlot = (float)s_TextureInsert + 1.0f;
				s_BoundTextures[s_TextureInsert] = texture; // Warning disabled at top of header.
				s_TextureInsert++;
			}

			float halfSize[2] = { size.x * 0.5f, size.y * 0.5f };
			s_QuadVBOffset[0].x = position.x - halfSize[0];
			s_QuadVBOffset[0].y = position.y - halfSize[1];
			s_QuadVBOffset[0].z = 0;
			s_QuadVBOffset[0].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[0].texCoords = { 0.0f, 0.0f };
			s_QuadVBOffset[0].textureSlot = textureSlot;

			s_QuadVBOffset[1].x = position.x + halfSize[0];
			s_QuadVBOffset[1].y = position.y - halfSize[1];
			s_QuadVBOffset[1].z = 0;
			s_QuadVBOffset[1].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[1].texCoords = { 1.0f, 0.0f };
			s_QuadVBOffset[1].textureSlot = textureSlot;

			s_QuadVBOffset[2].x = position.x + halfSize[0];
			s_QuadVBOffset[2].y = position.y + halfSize[1];
			s_QuadVBOffset[2].z = 0;
			s_QuadVBOffset[2].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[2].texCoords = { 1.0f, 1.0f };
			s_QuadVBOffset[2].textureSlot = textureSlot;

			s_QuadVBOffset[3].x = position.x - halfSize[0];
			s_QuadVBOffset[3].y = position.y + halfSize[1];
			s_QuadVBOffset[3].z = 0;
			s_QuadVBOffset[3].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[3].texCoords = { 0.0f, 1.0f };
			s_QuadVBOffset[3].textureSlot = textureSlot;

			s_QuadVBOffset += 4;
			s_QuadCount++;
		}



		void drawQuadSubTexture(const glm::vec2& position, const glm::vec2& size, const SharedPtr<SubTexture>& subTexture)
		{
			if (s_QuadCount >= MAX_QUADS || s_TextureInsert > 30)
			{
				renderBatched();
				resetBatch();
			}

			float textureSlot = 0.0f;
			for (int i = 0; i < 31; i++)
			{
				if (s_BoundTextures[i] == subTexture->getTexture())
				{
					textureSlot = (float)i;
					break;
				}
			}

			if (textureSlot == 0.0f)
			{
				textureSlot = (float)s_TextureInsert + 1.0f;
				s_BoundTextures[s_TextureInsert] = subTexture->getTexture(); // Warning disabled at top of header.
				s_TextureInsert++;
			}

			const glm::vec2* coords = subTexture->getTexCoords();
			float halfSize[2] = { size.x * 0.5f, size.y * 0.5f };
			s_QuadVBOffset[0].x = position.x - halfSize[0];
			s_QuadVBOffset[0].y = position.y - halfSize[1];
			s_QuadVBOffset[0].z = 0;
			s_QuadVBOffset[0].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[0].texCoords = coords[0];
			s_QuadVBOffset[0].textureSlot = textureSlot;

			s_QuadVBOffset[1].x = position.x + halfSize[0];
			s_QuadVBOffset[1].y = position.y - halfSize[1];
			s_QuadVBOffset[1].z = 0;
			s_QuadVBOffset[1].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[1].texCoords = coords[1];
			s_QuadVBOffset[1].textureSlot = textureSlot;

			s_QuadVBOffset[2].x = position.x + halfSize[0];
			s_QuadVBOffset[2].y = position.y + halfSize[1];
			s_QuadVBOffset[2].z = 0;
			s_QuadVBOffset[2].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[2].texCoords = coords[2];
			s_QuadVBOffset[2].textureSlot = textureSlot;

			s_QuadVBOffset[3].x = position.x - halfSize[0];
			s_QuadVBOffset[3].y = position.y + halfSize[1];
			s_QuadVBOffset[3].z = 0;
			s_QuadVBOffset[3].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[3].texCoords = coords[3];
			s_QuadVBOffset[3].textureSlot = textureSlot;

			s_QuadVBOffset += 4;
			s_QuadCount++;
		}
	

	}

}