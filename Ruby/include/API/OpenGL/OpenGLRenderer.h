#pragma once

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
			s_GladInitialized = true;


			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(debugCallbackFunc, nullptr);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

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
		
		void clear()
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}

	}

}