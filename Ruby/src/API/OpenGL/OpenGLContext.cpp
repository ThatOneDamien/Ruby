#include "ruby_pch.h"

#include "Ruby/Main/Core.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Ruby/Render/VertexArray.h"


// Forward Declaration for loading GL context into GLFW
extern "C" {
    typedef void (*GLFWglproc)(void);
    GLFWglproc glfwGetProcAddress(const char* procname);
}


namespace Ruby
{
    namespace OpenGLContext
    {
        static void debugCallbackFunc(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, const void*);

        void init()
        {
            int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            RB_ENSURE_CRITICAL(success, "Glad failed to initialize!");
            RB_ASSERT((GLVersion.major == 4 && GLVersion.minor > 5) || GLVersion.major > 4, 
                      "OpenGL version 4.6 or higher is required to run Ruby.");

            RB_INFO("OpenGL Version %s Initialized", glGetString(GL_VERSION));
            RB_INFO("OpenGL Vendor %s", glGetString(GL_VENDOR));
            RB_INFO("OpenGL Renderer %s", glGetString(GL_RENDERER));

            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(debugCallbackFunc, nullptr);

            glEnable(GL_DEPTH_TEST);
            // glDepthFunc(GL_LEQUAL);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        }

        int getBindableTextureSlots()
        {
            GLint maxTextureUnits;
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
            return maxTextureUnits;
        }

        void drawCall(const SharedPtr<VertexArray>& vao, uint32_t indexCount)
        {
            vao->bind();
            glDrawElements(GL_TRIANGLES, indexCount ? indexCount : vao->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
        }

        void setClearColor(float r, float g, float b)
        {
            glClearColor(r, g, b, 1.0f);
        }

        void setClearColor(uint8_t r, uint8_t g, uint8_t b)
        {
            glClearColor((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
        }

        void setClearColor(const glm::vec3& color)
        {
            glClearColor(color.r, color.g, color.b, 1.0f);
        }

        void setViewport(int x, int y, int width, int height)
        {
            glViewport(x, y, width, height);
        }

        void clear()
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }


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
    }
}
