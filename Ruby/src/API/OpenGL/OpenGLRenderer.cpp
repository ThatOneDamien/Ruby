#include "ruby_pch.h"

#include "Ruby/Main/Core.h"
#include "Ruby/Render/Renderer.h"

#ifdef RB_USE_OPENGL
#include <glad/glad.h>

// Forward Declaration for loading GL context into GLFW
extern "C" {
    typedef void (*GLFWglproc)(void);
    GLFWglproc glfwGetProcAddress(const char* procname);
}

namespace Ruby 
{
    namespace Renderer
    {
        namespace API 
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
            // void setGladLoadProc(GLADloadproc loadProc)
            // {
            //     if (!s_GladInitialized)
            //     {
            //         s_LoadProc = loadProc;
            //     }
            // }



            void initAPI()
            {
                if(s_GladInitialized)
                    return;
                
                int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
                RB_ASSERT_CRITICAL(success, "Glad failed to initialize!");
                RB_ASSERT((GLVersion.major == 4 && GLVersion.minor > 5) || GLVersion.major > 4, "OpenGL version 4.6 or higher is required to run Ruby.");

                RB_INFO("OpenGL Version %s Initialized", glGetString(GL_VERSION));
                RB_INFO("OpenGL Vendor %s", glGetString(GL_VENDOR));
                RB_INFO("OpenGL Renderer %s", glGetString(GL_RENDERER));
                s_GladInitialized = true;

                glEnable(GL_DEBUG_OUTPUT);
                glDebugMessageCallback(debugCallbackFunc, nullptr);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                //glEnable(GL_DEPTH_TEST);
                //glDepthFunc(GL_LESS);
            }

            void drawCall(const SharedPtr<VertexArray>& vao, uint32_t indexCount)
            {
                vao->bind();
                glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
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
                glClear(GL_COLOR_BUFFER_BIT);
            }

            int getBindableTextureSlots()
            {
                GLint maxTextureUnits;
                glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
                return maxTextureUnits;
            }

        }
    }
}
#endif
