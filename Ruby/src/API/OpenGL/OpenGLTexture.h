#pragma once

#include "Ruby/Render/Texture.h"


namespace Ruby 
{
    class OpenGLTexture : public Texture
    {
    public:
        OpenGLTexture(const std::string& filepath);
        OpenGLTexture(const TextureSpec& spec);
        OpenGLTexture(const void* data, const TextureSpec& spec);
    
        virtual ~OpenGLTexture();

        virtual void bind(uint8_t slot) const override;
        virtual void unbind() const override;
        virtual void setData(const void* data, uint32_t size) override;

        virtual inline int8_t getBoundSlot() const override { return m_BoundSlot; }
        virtual inline glm::vec2 getSize() const override { return { m_Width, m_Height }; }
        virtual inline const std::string& getFilepath() const override { return m_Filepath; }

    private: 

        RendererID m_RendererID = 0;
        mutable int8_t m_BoundSlot; // If set to -1, texture is currently not bound
        uint32_t m_FormatIntern, m_FormatBase;
        uint32_t m_Width, m_Height, m_BPP;
        std::string m_Filepath;
    };

}