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

		// 36 bytes in size, composed of position, color, texture coordinates, and texture slot.
		struct TextVertex
		{
			union
			{
				struct { float x, y, z; };
				glm::vec3 position;
			};

			glm::vec4 color;
			glm::vec2 texCoords;

		};

		static constexpr uint16_t MAX_QUADS = 1000;
		static constexpr size_t MAX_VERTICES = MAX_QUADS * 4;
		static constexpr size_t MAX_INDICES = MAX_QUADS * 6;

		static SharedPtr<VertexArray> s_QuadVAO{ nullptr };
		static SharedPtr<VertexBuffer> s_QuadVBO{ nullptr };
		static SharedPtr<IndexBuffer> s_QuadTextIBO{ nullptr };
		static SharedPtr<Shader> s_QuadShader{ nullptr };

		static QuadVertex* s_QuadVBData{ nullptr };
		static QuadVertex* s_QuadVBOffset{ nullptr };
		static uint16_t s_QuadCount = 0;

		static SharedPtr<VertexArray> s_TextVAO{ nullptr };
		static SharedPtr<VertexBuffer> s_TextVBO{ nullptr };
		static SharedPtr<Shader> s_TextShader{ nullptr };
		static SharedPtr<Font> s_DefaultFont{ nullptr };

		// TODO : Abstract the size of array into a system that auto detects the max number of texture slots for the card.
		// for now it is 31, with one of the slots being reserved for basic white 1 pixel texture for colored quads.
		static SharedPtr<Texture> s_BoundTextures[31] = { nullptr };
		static SharedPtr<Texture> s_BlankColorTexture{ nullptr };
		static uint8_t s_TextureInsert = 0;

		static const Camera* s_MainCam;
		static SharedPtr<UniformBuffer> s_CamUBO{ nullptr };

		static TextVertex* s_TextVBData{ nullptr };
		static TextVertex* s_TextVBOffset{ nullptr };
		static uint16_t s_TextCount = 0;

		// ---------------END BATCH RENDERER STUFF-----------------


		void init()
		{
			API::initAPI();

			// QUAD STUFF
			
			s_QuadVAO = VertexArray::createVAO();
			s_QuadVBO = VertexBuffer::createVBO(MAX_VERTICES * sizeof(QuadVertex));

			{
				VertexLayout layout;
				layout.push({ LayoutType::Float, 3, false }); // Position
				layout.push({ LayoutType::Float, 4, false }); // Color
				layout.push({ LayoutType::Float, 2, false }); // Texture Coords
				layout.push({ LayoutType::Float, 1, false }); // Texture Slot

				s_QuadVBO->setLayout(layout);
			}

			s_QuadVAO->setVertexBuffer(s_QuadVBO);


			s_QuadVBData = new QuadVertex[MAX_VERTICES];
			s_QuadVBOffset = s_QuadVBData;

			{
				uint32_t indices[MAX_INDICES];

				// Send all the indices as a static buffer to the GPU, as
				// we can already know which indices will match to each quad
				// even before they are drawn. This is because each quad follows
				// the same pattern of indices, and they only have 4 indices each,
				// with 2 occurring twice.
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

				s_QuadTextIBO = IndexBuffer::createIBO(indices, MAX_INDICES);
			}

			s_QuadVAO->setIndexBuffer(s_QuadTextIBO);

			
			// TEXT STUFF

			s_TextVAO = VertexArray::createVAO();
			s_TextVBO = VertexBuffer::createVBO(MAX_VERTICES * sizeof(TextVertex));

			{
				VertexLayout layout;
				layout.push({ LayoutType::Float, 3, false }); // Positions
				layout.push({ LayoutType::Float, 4, false }); // Color
				layout.push({ LayoutType::Float, 2, false }); // Texture Coords

				s_TextVBO->setLayout(layout);
			}

			s_TextVAO->setVertexBuffer(s_TextVBO);


			s_TextVBData = new TextVertex[MAX_VERTICES];
			s_TextVBOffset = s_TextVBData;

			// We render text as quads, and the sizes are the same, so we can
			// reuse the same index buffer object to draw both text and quads
			s_TextVAO->setIndexBuffer(s_QuadTextIBO);

			s_BlankColorTexture = Texture::createTexture();

			// This sets the data of the white texture to 1 pixel of white, which allows simple
			// colored quads with no separate texture all within the batch.
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

			s_DefaultFont = createShared<Font>("res/fonts/Roboto-Regular.ttf", 60);

			s_TextShader = Shader::createShader("res/shaders/text.glsl");

		}



		void deInit()
		{
			delete[] s_QuadVBData;
			API::deInitAPI();
		}

		// Sets main camera of the scene
		void updateCamera(const Camera& cam)
		{
			s_MainCam = &cam;
		}

		void renderSubmit(const SharedPtr<VertexArray>& vao, const SharedPtr<Shader> shader)
		{
			shader->bind();
			//shader->setUniformMat4("u_ViewProj", scene->cam.viewProj() or something like that);

			API::drawCall(vao);
		}



		void renderBatched()
		{
			if (s_MainCam)
			{
				const glm::mat4& viewProj = s_MainCam->getViewProjectionMatrix();
				s_CamUBO->setData(&viewProj[0][0], sizeof(glm::mat4), 0);
			}

			if (s_QuadCount) 
			{
				s_QuadVAO->bind();
				s_QuadVBO->setVertexData(s_QuadVBData, (uint32_t)((uint8_t*)s_QuadVBOffset - (uint8_t*)s_QuadVBData), 0);
				s_QuadShader->bind();

				s_BlankColorTexture->bind(0);
				for (int i = 0; i < s_TextureInsert; i++)
					s_BoundTextures[i]->bind(i + 1);

				API::drawCall(s_QuadVAO);
			}

			if (s_TextCount) 
			{
				s_TextVAO->bind();
				s_TextVBO->setVertexData(s_TextVBData, (uint32_t)((uint8_t*)s_TextVBOffset - (uint8_t*)s_TextVBData), 0);
				s_TextShader->bind();

				s_DefaultFont->getAtlasTexture()->bind(0);

				API::drawCall(s_TextVAO);
			}



			s_QuadCount = 0;
			s_TextCount = 0;

			s_QuadVBOffset = s_QuadVBData;
			s_TextVBOffset = s_TextVBData;


			s_TextureInsert = 1;

		}



		void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
		{
			if (s_QuadCount >= MAX_QUADS)
			{
				renderBatched();
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

			const glm::vec4& coords = subTexture->getTexCoords();
			float halfSize[2] = { size.x * 0.5f, size.y * 0.5f };
			s_QuadVBOffset[0].x = position.x - halfSize[0];
			s_QuadVBOffset[0].y = position.y - halfSize[1];
			s_QuadVBOffset[0].z = 0;
			s_QuadVBOffset[0].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[0].texCoords = { coords.x, coords.y };
			s_QuadVBOffset[0].textureSlot = textureSlot;

			s_QuadVBOffset[1].x = position.x + halfSize[0];
			s_QuadVBOffset[1].y = position.y - halfSize[1];
			s_QuadVBOffset[1].z = 0;
			s_QuadVBOffset[1].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[1].texCoords = { coords.z, coords.y };
			s_QuadVBOffset[1].textureSlot = textureSlot;

			s_QuadVBOffset[2].x = position.x + halfSize[0];
			s_QuadVBOffset[2].y = position.y + halfSize[1];
			s_QuadVBOffset[2].z = 0;
			s_QuadVBOffset[2].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[2].texCoords = { coords.z, coords.w };
			s_QuadVBOffset[2].textureSlot = textureSlot;

			s_QuadVBOffset[3].x = position.x - halfSize[0];
			s_QuadVBOffset[3].y = position.y + halfSize[1];
			s_QuadVBOffset[3].z = 0;
			s_QuadVBOffset[3].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[3].texCoords = { coords.x, coords.w };
			s_QuadVBOffset[3].textureSlot = textureSlot;

			s_QuadVBOffset += 4;
			s_QuadCount++;
		}
	

		void drawText(const std::string& str, const glm::vec2& position, float size, const glm::vec4& color)
		{

			FontMetrics metrics = s_DefaultFont->getMetrics();

			float x = position.x;
			float y = position.y;

			for (int i = 0; i < str.size(); i++)
			{
				if (s_TextCount >= MAX_QUADS)
				{
					renderBatched();
				}

				if (str[i] == '\n')
				{
					x = position.x;
					y -= size * metrics.Height * 1.5;
					continue;
				}
				if (str[i] == ' ')
				{
					x += size * metrics.SpaceSize;
					continue;
				}
				if (str[i] == '\t')
				{
					x += size * metrics.SpaceSize * 4;
					continue;
				}

				CharMetrics charMetrics = s_DefaultFont->getCharMetrics(str[i]);

				if (!charMetrics.SubTex)
				{
					charMetrics = s_DefaultFont->getCharMetrics('?');
				}
				if (!charMetrics.SubTex)
				{
					RB_ERROR("Unknown symbol. \'?\' is also missing in the font.");
					return;
				}

				glm::vec2 quadPos 
				{
					x + charMetrics.Bearing.x * size, 
					y - (charMetrics.Size.y - charMetrics.Bearing.y) * size 
				};

				float w = charMetrics.Size.x * size;
				float h = charMetrics.Size.y * size;

				const glm::vec4& coords = charMetrics.SubTex->getTexCoords();

				s_TextVBOffset[0].x = quadPos.x;
				s_TextVBOffset[0].y = quadPos.y;
				s_TextVBOffset[0].z = 0;
				s_TextVBOffset[0].color = color;
				s_TextVBOffset[0].texCoords = { coords.x, coords.y };
				  
				s_TextVBOffset[1].x = quadPos.x + w;
				s_TextVBOffset[1].y = quadPos.y;
				s_TextVBOffset[1].z = 0;
				s_TextVBOffset[1].color = color;
				s_TextVBOffset[1].texCoords = { coords.z, coords.y };
				  
				s_TextVBOffset[2].x = quadPos.x + w;
				s_TextVBOffset[2].y = quadPos.y + h;
				s_TextVBOffset[2].z = 0;
				s_TextVBOffset[2].color = color;
				s_TextVBOffset[2].texCoords = { coords.z, coords.w };
				  
				s_TextVBOffset[3].x = quadPos.x;
				s_TextVBOffset[3].y = quadPos.y + h;
				s_TextVBOffset[3].z = 0;
				s_TextVBOffset[3].color = color;
				s_TextVBOffset[3].texCoords = { coords.x, coords.w };
				  
				s_TextVBOffset += 4;
				s_TextCount++;

				x += charMetrics.Advance * size;
			}
		}

	}

}