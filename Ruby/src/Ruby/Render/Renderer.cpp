#include "ruby_pch.h"

#include <glm/gtc/matrix_transform.hpp>
#undef INFINITE
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <msdf-atlas-gen/FontGeometry.h>
#include <msdf-atlas-gen/GlyphGeometry.h>

#include "Ruby/Main/Core.h"
#include "Ruby/Render/Renderer.h"
#include "Ruby/Render/Font.h"
#include "Ruby/Main/App.h"


#pragma warning(disable: 6385) // Compiler warning about overflowing buffer which wont happen.
#pragma warning(disable: 6386)

namespace Ruby 
{
	struct FontData
	{
		std::vector<msdf_atlas::GlyphGeometry> Glyphs;
		msdf_atlas::FontGeometry FontGeometry;
	};

	namespace Renderer
	{
		namespace API
		{
			void initAPI();
			void deInitAPI();
			int getBindableTextureSlots();
		}

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
		static constexpr glm::vec4 VERTEX_POSITIONS[] =
		{
			{-0.5f, -0.5f, 0.0f, 1.0f},
			{ 0.5f, -0.5f, 0.0f, 1.0f},
			{ 0.5f,  0.5f, 0.0f, 1.0f},
			{-0.5f,  0.5f, 0.0f, 1.0f},
		};

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
		static SharedPtr<Font> s_CurrentFont{ nullptr };

		static SharedPtr<Texture>* s_BoundTextures{ nullptr }; // Size determined below
		static SharedPtr<Texture> s_BlankColorTexture{ nullptr };
		static int s_TextureSlotCount = -1;
		static uint8_t s_TextureInsert = 0;

		static const Camera* s_CameraInUse{nullptr};
		static SharedPtr<UniformBuffer> s_CamUBO{ nullptr };

		static TextVertex* s_TextVBData{ nullptr };
		static TextVertex* s_TextVBOffset{ nullptr };
		static uint16_t s_TextCount = 0;

		// ---------------END BATCH RENDERER STUFF-----------------


		void init()
		{
			API::initAPI();

			// Queries GPU for slot count and creates array to hold that many bound textures
			s_TextureSlotCount = API::getBindableTextureSlots();
			s_TextureSlotCount = s_TextureSlotCount > 32 ? 32 : s_TextureSlotCount;
			s_BoundTextures = new SharedPtr<Texture>[s_TextureSlotCount - 1];

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
					indices[i]     = offset;
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

			s_BlankColorTexture = Texture::createTexture(TextureSpec());

			// This sets the data of the white texture to 1 pixel of white, which allows simple
			// colored quads with no separate texture all within the batch.
			uint32_t white = 0xFFFFFFFF;
			s_BlankColorTexture->setData((const void*)&white, sizeof(uint32_t));

			s_QuadShader = Shader::createShader("res/shaders/renderer/quad.glsl");

			s_QuadShader->bind();
			s_CamUBO = UniformBuffer::createUBO(sizeof(glm::mat4), 0);
			{
				int arr[32];
				for (int i = 0; i < s_TextureSlotCount; ++i)
					arr[i] = i;

				s_QuadShader->setUniformIntArray("u_Textures", s_TextureSlotCount, arr);
			}

			s_TextShader = Shader::createShader("res/shaders/renderer/text.glsl");

		}



		void deInit()
		{
			delete[] s_QuadVBData;
			API::deInitAPI();
		}

		void useCamera(const Camera& cam)
		{
			s_CameraInUse = &cam;
		}

		void useFont(const SharedPtr<Font>& font)
		{
			s_CurrentFont = font;
		}

		void renderSubmit(const SharedPtr<VertexArray>& vao, const SharedPtr<Shader> shader)
		{
			shader->bind();

			API::drawCall(vao, vao->getIndexBuffer()->getCount());
		}

		void resetBatch()
		{
			s_QuadCount = 0;
			s_TextCount = 0;

			s_QuadVBOffset = s_QuadVBData;
			s_TextVBOffset = s_TextVBData;


			s_TextureInsert = 0;
		}

		void renderBatch(const Camera* cam)
		{
			cam = cam ? cam : s_CameraInUse;
			if (cam)
				s_CamUBO->setData(&cam->getViewProjectionMatrix()[0][0], sizeof(glm::mat4), 0);
			else
			{
				RB_WARN("Renderer was not provided a camera.");
			}

			if (s_QuadCount)
			{
				s_QuadVAO->bind();
				s_QuadVBO->setVertexData(s_QuadVBData, (uint32_t)((uint8_t*)s_QuadVBOffset - (uint8_t*)s_QuadVBData), 0);
				s_QuadShader->bind();

				s_BlankColorTexture->bind(0);
				for (int i = 0; i < s_TextureInsert; ++i)
					s_BoundTextures[i]->bind(i + 1);

				API::drawCall(s_QuadVAO, s_QuadCount * 6);
			}

			if (s_TextCount && s_CurrentFont)
			{
				s_TextVAO->bind();
				s_TextVBO->setVertexData(s_TextVBData, (uint32_t)((uint64_t)s_TextVBOffset - (uint64_t)s_TextVBData), 0);
				s_TextShader->bind();

				s_CurrentFont->getAtlasTexture()->bind(0);

				API::drawCall(s_TextVAO, s_TextCount * 6);
			}

			resetBatch();
		}


	namespace internal 
	{
		float getTextureSlotOrAdd(const SharedPtr<Texture>& tex)
		{
			float res = 0;
			for (int i = 0; i < s_TextureInsert; ++i)
			{
				if (s_BoundTextures[i] == tex)
				{
					res = (float)i + 1.0f;
					break;
				}
			}

			if (!res)
			{
				if (s_TextureInsert >= s_TextureSlotCount - 2)
					renderBatch();

				res = (float)s_TextureInsert + 1.0f;
				s_BoundTextures[s_TextureInsert] = tex;
				++s_TextureInsert;
			}

			return res;
		}

		void drawQuadBasic(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, 
			               const SharedPtr<Texture>& tex, const TexCoords& texCoords = TexCoords())
		{
			if (s_QuadCount >= MAX_QUADS)
			{
				renderBatch();
			}

			float texSlot = (tex == nullptr) ? 0.0f : getTextureSlotOrAdd(tex);

			for (size_t i = 0; i < 4; ++i)
			{
				s_QuadVBOffset[i].x = position.x + VERTEX_POSITIONS[i].x * size.x;
				s_QuadVBOffset[i].y = position.y + VERTEX_POSITIONS[i].y * size.y;
				s_QuadVBOffset[i].z = (float)s_QuadCount / (float)MAX_QUADS;
				s_QuadVBOffset[i].color = color;
				s_QuadVBOffset[i].texCoords = texCoords[i];
				s_QuadVBOffset[i].textureSlot = texSlot;
			}

			s_QuadVBOffset += 4;
			++s_QuadCount;
		}

		void drawQuadRotBasic(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color,
							  const SharedPtr<Texture>& tex, const TexCoords& texCoords = TexCoords())
		{
			if (s_QuadCount >= MAX_QUADS)
			{
				renderBatch();
			}

			float texSlot = (tex == nullptr) ? 0.0f : getTextureSlotOrAdd(tex);

			glm::mat4 transform = 
				glm::scale(
					glm::rotate(
						glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f }), 
						rotation, 
						{ 0.0f, 0.0f, 1.0f }
					), 
					{ size.x, size.y, 1.0f }
				);

			for (size_t i = 0; i < 4; ++i)
			{
				glm::vec4 pos = transform * VERTEX_POSITIONS[i];

				s_QuadVBOffset[i].x = pos.x;
				s_QuadVBOffset[i].y = pos.y;
				s_QuadVBOffset[i].z = (float)s_QuadCount / (float)MAX_QUADS;
				s_QuadVBOffset[i].color = color;
				s_QuadVBOffset[i].texCoords = texCoords[i];
				s_QuadVBOffset[i].textureSlot = texSlot;
			}

			s_QuadVBOffset += 4;
			++s_QuadCount;
		}
	} // End namespace internal


		void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
		{
			internal::drawQuadBasic(position, size, color, nullptr);
		}



		void drawQuadTex(const glm::vec2& position, const glm::vec2& size, 
						const SharedPtr<Texture>& texture, const glm::vec4& color)
		{
			internal::drawQuadBasic(position, size, color, texture);
		}



		void drawQuadSubTex(const glm::vec2& position, const glm::vec2& size, 
							const SharedPtr<SubTexture>& subTexture, const glm::vec4& color)
		{
			internal::drawQuadBasic(position, size, color, subTexture->getTexture(), subTexture->getTexCoords());
		}

		void drawQuadRot(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
		{
			internal::drawQuadRotBasic(position, size, rotation, color, nullptr);
		}

		void drawQuadRotTex(const glm::vec2& position, const glm::vec2& size, float rotation,
			const SharedPtr<Texture>& texture, const glm::vec4& color)
		{
			internal::drawQuadRotBasic(position, size, rotation, color, texture);
		}

		void drawQuadRotSubTex(const glm::vec2& position, const glm::vec2& size, float rotation,
			const SharedPtr<SubTexture>& subTexture, const glm::vec4& color)
		{
			internal::drawQuadRotBasic(position, size, rotation, color, subTexture->getTexture(), subTexture->getTexCoords());
		}
	

		void drawText(const std::string& str, const glm::vec2& position, float scale, float rotation, const glm::vec4& color)
		{
			if (!s_CurrentFont)
			{
				RB_WARN("Attempted to draw a string of text without loading a font first. "
						"Call loadFont() to load a font for the renderer to use.");
				return;
			}
			const msdf_atlas::FontGeometry& fontGeometry = s_CurrentFont->getAtlasData()->FontGeometry;
			const msdfgen::FontMetrics& metrics = fontGeometry.getMetrics();

			double x = position.x;
			double y = position.y;

			const double lineScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
			const double spaceAdvance = fontGeometry.getGlyph(' ')->getAdvance();

			for (size_t i = 0; i < str.size(); ++i)
			{
				if (s_TextCount >= MAX_QUADS)
				{
					renderBatch();
				}
				switch (str[i])
				{
				case '\n':
					x = position.x;
					y -= lineScale * metrics.lineHeight * 1.5;
					break;
				case ' ':
				{
					double advance = spaceAdvance;
					if (i < str.size() - 1)
					{
						fontGeometry.getAdvance(advance, str[i], str[i+1]);
					}
					x += lineScale * advance;
					break;
				}
				case '\t':
					x += lineScale * spaceAdvance * 4;
					break;
				case '\r':
					break;
				default:
					const msdf_atlas::GlyphGeometry* glyph = fontGeometry.getGlyph(str[i]);

					if (!glyph)
						glyph = fontGeometry.getGlyph('?');
					if (!glyph)
					{
						RB_WARN("Unknown symbol. \'?\' is also missing in the font.");
						continue;
					}

					double leftA, bottomA, rightA, topA;
					glyph->getQuadAtlasBounds(leftA, bottomA, rightA, topA);

					double leftP, bottomP, rightP, topP;
					glyph->getQuadPlaneBounds(leftP, bottomP, rightP, topP);

					leftP = leftP * lineScale + x;
					bottomP = bottomP * lineScale + y;
					rightP = rightP * lineScale + x;
					topP = topP * lineScale + y;

					glm::vec2 texSize = s_CurrentFont->getAtlasTexture()->getSize();

					float texelWidth = 1.0f / texSize.x;
					float texelHeight = 1.0f / texSize.y;
					leftA *= texelWidth;
					rightA *= texelWidth;
					bottomA *= texelHeight;
					topA *= texelHeight;

					glm::mat4 transform = 
						glm::scale(
							glm::rotate(
								glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f }),
								rotation,
								{ 0.0f, 0.0f, 1.0f }
							),
							{ scale, scale, 1.0f }
						);

					glm::vec4 temp = transform * glm::vec4(leftP, bottomP, 0.0f, 1.0f);
					s_TextVBOffset[0].position = { temp.x, temp.y, 0.0f };
					s_TextVBOffset[0].color = color;
					s_TextVBOffset[0].texCoords = { leftA, bottomA };

					temp = transform * glm::vec4(leftP, topP, 0.0f, 1.0f);
					s_TextVBOffset[1].position = { temp.x, temp.y, 0.0f };
					s_TextVBOffset[1].color = color;
					s_TextVBOffset[1].texCoords = { leftA, topA };

					temp = transform * glm::vec4(rightP, topP, 0.0f, 1.0f);
					s_TextVBOffset[2].position = { temp.x, temp.y, 0.0f };
					s_TextVBOffset[2].color = color;
					s_TextVBOffset[2].texCoords = { rightA, topA };

					temp = transform * glm::vec4(rightP, bottomP, 0.0f, 1.0f);
					s_TextVBOffset[3].position = { temp.x, temp.y, 0.0f };
					s_TextVBOffset[3].color = color;
					s_TextVBOffset[3].texCoords = { rightA, bottomA };

					s_TextVBOffset += 4;
					++s_TextCount;
					
					if (i < str.size() - 1)
					{
						double advance = glyph->getAdvance();
						fontGeometry.getAdvance(advance, str[i], str[i + 1]);

						x += lineScale * advance;
					}
				}
			}
		}

	} // End namespace Renderer

} // End namespace Ruby