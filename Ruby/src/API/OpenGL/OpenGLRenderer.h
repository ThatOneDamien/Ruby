#pragma once

#pragma warning(disable: 6386) // Compiler warning about overflowing buffer which wont happen.
#ifdef RB_RENDERER_GL
#include "Ruby/Main/Core.h"

#include "Ruby/Render/Renderer.h"
#include <glad/glad.h>

namespace Ruby {

	namespace Renderer
	{
		static void debugCallbackFunc(
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam
		)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_LOW:
				RB_WARN("OpenGL Warning: %s", message);
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				RB_ERROR("OpenGL Error: %s", message);
				break;
			case GL_DEBUG_SEVERITY_HIGH:
				RB_CRITICAL("OpenGL Critical Error: %s", message);
				break;
			}

		}

		static bool s_GladInitialized = false;
		static GLADloadproc s_LoadProc = nullptr;

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

		static GLuint s_QuadVAO = 0;

		static GLuint s_QuadVBO = 0;
		static QuadVertex* s_QuadVBData = nullptr;
		static QuadVertex* s_QuadVBOffset = nullptr;
		static std::shared_ptr<Shader> s_QuadShader = nullptr;

		static GLuint s_QuadIBO = 0;
		static uint16_t s_QuadCount = 0;

		// TODO : Abstract the size of array into a system that auto detects the max number of texture slots for the card.
		// for now it is 31, with one of the slots being reserved for basic white 1 pixel texture for colored quads.
		static std::shared_ptr<Texture> s_BoundTextures[31] = {nullptr};
		static uint8_t s_TextureInsert = 0;
		static std::shared_ptr<Texture> s_BlankColorTexture;

		// ---------------END BATCH RENDERER STUFF-----------------

		// Should only be called once prior to Renderer Initialization
		// Called in order to avoid including GLFW in this file.
		void setGladLoadProc(GLADloadproc loadProc)
		{
			if (!s_GladInitialized)
			{
				s_LoadProc = loadProc;
			}
		}

		void init()
		{
			RB_ASSERT(!s_GladInitialized, "Glad initialized more than once.");
			RB_ASSERT(s_LoadProc, "Load proc is nullptr, maybe you forgot to call setGladLoadProc()?");
			int success = gladLoadGLLoader(s_LoadProc);
			RB_ASSERT(success, "Glad failed to initialize!");
			RB_ASSERT((GLVersion.major == 4 && GLVersion.minor > 5) || GLVersion.major > 4, "OpenGL version 4.6 or higher is required to run Ruby.");

			RB_INFO("OpenGL Version %s Initialized", glGetString(GL_VERSION));
			RB_INFO("OpenGL Vendor %s", glGetString(GL_VENDOR));
			RB_INFO("OpenGL Renderer %s", glGetString(GL_RENDERER));
			s_GladInitialized = true;


			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(debugCallbackFunc, nullptr);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glCreateVertexArrays(1, &s_QuadVAO);
			glCreateBuffers(1, &s_QuadVBO);
			glCreateBuffers(1, &s_QuadIBO);

			uint32_t indices[MAX_INDICES];
			s_QuadVBData = new QuadVertex[MAX_VERTICES];

			// Pattern for indices 0, 1, 2, 2, 3, 0 for drawing a basic quad. This will always be the same no matter what so it
			// is better to only send the data to the graphics card once and not update it ass quads are added.
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

			glBindVertexArray(s_QuadVBO);
			glBindBuffer(GL_ARRAY_BUFFER, s_QuadVBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_QuadIBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertex) * MAX_VERTICES, nullptr, GL_DYNAMIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * MAX_INDICES, indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)0);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)(sizeof(float) * 3));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)(sizeof(float) * 7));
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)(sizeof(float) * 9));

			glEnableVertexArrayAttrib(s_QuadVAO, 0);
			glEnableVertexArrayAttrib(s_QuadVAO, 1);
			glEnableVertexArrayAttrib(s_QuadVAO, 2);
			glEnableVertexArrayAttrib(s_QuadVAO, 3);

			s_BlankColorTexture = Texture::createTexture(1, 1);
			
			// This sets the data of the white texture to 1 pixel of white, which allows the card to multiply by color to produce
			// purely colored quads with no texture, while still allowing batching.
			uint32_t white = 0xFFFFFFFF;
			s_BlankColorTexture->setData((const void*)&white, sizeof(uint32_t));

			s_QuadShader = Shader::createShader("res/shaders/quad_shader.glsl");

			s_QuadShader->bind();
			int arr[32];
			for (int i = 0; i < 32; i++)
				arr[i] = i;

			s_QuadShader->setUniformIntArray("u_Textures", 32, arr);
		}

		void deInit()
		{
			glDeleteVertexArrays(1, &s_QuadVAO);
			glDeleteBuffers(1, &s_QuadVBO);
			glDeleteBuffers(1, &s_QuadIBO);

			delete[] s_QuadVBData;
			s_QuadShader.reset();
			s_BlankColorTexture.reset();
			for (int i = 0; i < 31; i++)
				if (s_BoundTextures[i])
					s_BoundTextures[i].reset();
		}

		void renderIndices(const std::shared_ptr<VertexArray>& vao, const std::shared_ptr<Shader> shader, const glm::mat4& transform)
		{
			shader->bind();

			vao->bind();
			glDrawElements(GL_TRIANGLES, vao->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
		}

		void resetBatch()
		{
			s_QuadVBOffset = s_QuadVBData;
		}

		void renderBatched()
		{
			glNamedBufferSubData(s_QuadVBO, 0, (GLsizeiptr)((uint8_t*)s_QuadVBOffset - (uint8_t*)s_QuadVBData), s_QuadVBData);
			glBindVertexArray(s_QuadVAO);
			s_QuadShader->bind();

			s_BlankColorTexture->bind(0);
			for (int i = 0; i < 31 && s_BoundTextures[i]; i++)
				s_BoundTextures[i]->bind(i+1);

			glDrawElements(GL_TRIANGLES, s_QuadCount * 6, GL_UNSIGNED_INT, nullptr);

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

			s_QuadVBOffset[0].x = position.x;
			s_QuadVBOffset[0].y = position.y;
			s_QuadVBOffset[0].z = 0;
			s_QuadVBOffset[0].color = color;
			s_QuadVBOffset[0].texCoords = { 0.0f, 0.0f };
			s_QuadVBOffset[0].textureSlot = 0.0f;

			s_QuadVBOffset[1].x = position.x + size.x;
			s_QuadVBOffset[1].y = position.y;
			s_QuadVBOffset[1].z = 0;
			s_QuadVBOffset[1].color = color;
			s_QuadVBOffset[1].texCoords = { 1.0f, 0.0f };
			s_QuadVBOffset[1].textureSlot = 0.0f;

			s_QuadVBOffset[2].x = position.x + size.x;
			s_QuadVBOffset[2].y = position.y + size.y;
			s_QuadVBOffset[2].z = 0;
			s_QuadVBOffset[2].color = color;
			s_QuadVBOffset[2].texCoords = { 1.0f, 1.0f };
			s_QuadVBOffset[2].textureSlot = 0.0f;

			s_QuadVBOffset[3].x = position.x;
			s_QuadVBOffset[3].y = position.y + size.y;
			s_QuadVBOffset[3].z = 0;
			s_QuadVBOffset[3].color = color;
			s_QuadVBOffset[3].texCoords = { 0.0f, 1.0f };
			s_QuadVBOffset[3].textureSlot = 0.0f;

			s_QuadVBOffset += 4;
			s_QuadCount++;
		}

		void drawTexture(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture)
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
				textureSlot = (float)s_TextureInsert+1.0f;
				s_BoundTextures[s_TextureInsert] = texture; // Warning disabled at top of header.
				s_TextureInsert++;
			}

			s_QuadVBOffset[0].x = position.x;
			s_QuadVBOffset[0].y = position.y;
			s_QuadVBOffset[0].z = 0;
			s_QuadVBOffset[0].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[0].texCoords = { 0.0f, 0.0f };
			s_QuadVBOffset[0].textureSlot = textureSlot;

			s_QuadVBOffset[1].x = position.x + size.x;
			s_QuadVBOffset[1].y = position.y;
			s_QuadVBOffset[1].z = 0;
			s_QuadVBOffset[1].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[1].texCoords = { 1.0f, 0.0f };
			s_QuadVBOffset[1].textureSlot = textureSlot;

			s_QuadVBOffset[2].x = position.x + size.x;
			s_QuadVBOffset[2].y = position.y + size.y;
			s_QuadVBOffset[2].z = 0;
			s_QuadVBOffset[2].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[2].texCoords = { 1.0f, 1.0f };
			s_QuadVBOffset[2].textureSlot = textureSlot;

			s_QuadVBOffset[3].x = position.x;
			s_QuadVBOffset[3].y = position.y + size.y;
			s_QuadVBOffset[3].z = 0;
			s_QuadVBOffset[3].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			s_QuadVBOffset[3].texCoords = { 0.0f, 1.0f };
			s_QuadVBOffset[3].textureSlot = textureSlot;

			s_QuadVBOffset += 4;
			s_QuadCount++;
		}

		// GL CONTEXT STUFF

		void setClearColor(float r, float g, float b, float a)
		{
			glClearColor(r, g, b, a);
		}

		void setClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
		}

		void setClearColor(const glm::vec4& color)
		{
			glClearColor(color.r, color.g, color.g, color.a);
		}

		void setViewport(int x, int y, int width, int height)
		{
			glViewport(x, y, width, height);
		}

		void clear()
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}


	}

}
#endif