#include "ruby_pch.h"

#include "Cubemap.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Ruby
{
    OpenGLCubemap::OpenGLCubemap(const CubemapTexPaths& paths)
    {
        stbi_set_flip_vertically_on_load(false);
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
        
        for(int i = 0; i < 6; ++i)
        {
            uint8_t* data = nullptr;
            int width, height, bpp;

            data = stbi_load((&paths.Right + i)->c_str(), &width, &height, &bpp, 3);
            RB_ENSURE_RET_VOID(data && bpp, "Unable to load texture at path %s", (&paths.Right + i)->c_str());
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

    }

    void OpenGLCubemap::bind() const
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
    }
}
