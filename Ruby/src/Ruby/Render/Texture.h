#pragma once

#include <glm/glm.hpp>

#include "Ruby/Main/Core.h"

namespace Ruby 
{
    // The way the numbers are set up are for the purpose of what follows:
    // 
    // By bit shifting these numbers to the right 3 times, the number will
    // be equivalent to the OpenGL internal format defines.
    //
    // Also by using the bitwise and with this number and 7, the result will
    // be the number of bytes per channel this format contains.
    // 
    enum class PixelFormat : uint32_t
    {
        R8    = (0x8229 << 3) + 1,
        RG8   = (0x822B << 3) + 2,
        RGB8  = (0x8051 << 3) + 3,
        RGBA8 = (0x8058 << 3) + 4
    };


    // FROM OPENGL
    enum class TextureFilter
    {
        Nearest = 0x2600,
        Linear  = 0x2601,
    };

    // FROM OPENGL
    enum class TextureWrap
    {
        ClampToEdge       = 0x812F,
        ClampToBorder     = 0x812D,
        MirroredRepeat    = 0x8370,
        Repeat            = 0x2901,
        MirrorClampToEdge = 0x8743
    };

    struct TextureSpec
    {
        uint32_t Width = 1;
        uint32_t Height = 1;
        PixelFormat Format = PixelFormat::RGBA8;
        TextureFilter MinFilter = TextureFilter::Linear; // Minimizing function for when the texture needs to be rendered at a smaller size.
        TextureFilter MagFilter = TextureFilter::Linear; // Magnifying function for when the texture needs to be rendered at a larger size.
        TextureWrap WrapS = TextureWrap::ClampToEdge;
        TextureWrap WrapT = TextureWrap::ClampToEdge;
        bool MipMapping = false;
    };

    class RB_NOVTABLE Texture
    {
    public:
        virtual ~Texture() = default;
        
        virtual void bind(uint8_t slot) const = 0;
        virtual void unbind() const = 0;
        virtual void setData(const void* data, uint32_t size) = 0;

        virtual inline int8_t getBoundSlot() const = 0;
        virtual inline glm::vec2 getSize() const = 0;
        virtual inline const std::string& getFilepath() const = 0;

        static SharedPtr<Texture> create(const std::string& filepath, const TextureSpec& spec = {});
        static SharedPtr<Texture> create(const TextureSpec& spec);
        static SharedPtr<Texture> create(const void* data, const TextureSpec& spec);
    };


    struct TexCoords
    {
        TexCoords(const glm::vec2& bottomLeftCorner, const glm::vec2& topRightCorner)
        {
            m_TexCoords[0] = { bottomLeftCorner.x, bottomLeftCorner.y };
            m_TexCoords[1] = { topRightCorner.x, bottomLeftCorner.y };
            m_TexCoords[2] = { topRightCorner.x, topRightCorner.y };
            m_TexCoords[3] = { bottomLeftCorner.x, topRightCorner.y };
        }
        
        TexCoords()
        {
            m_TexCoords[0] = QUAD_TEX_COORDS[0];
            m_TexCoords[1] = QUAD_TEX_COORDS[1];
            m_TexCoords[2] = QUAD_TEX_COORDS[2];
            m_TexCoords[3] = QUAD_TEX_COORDS[3];
        }

        const glm::vec2& operator[](size_t index) const
        {
            RB_ASSERT(index < 4, "Index out of bounds.");

            return m_TexCoords[index];
        }

        glm::vec2& operator[](size_t index)
        {
            RB_ASSERT(index > -1 && index < 4, "Index out of bounds.");

            return m_TexCoords[index];
        }

    private:
        glm::vec2 m_TexCoords[4];

        static constexpr glm::vec2 QUAD_TEX_COORDS[] =
        {
            {0.0f, 0.0f},
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f }
        };

    };

    class SubTexture
    {
    public:
        inline SubTexture(const SharedPtr<Texture>& texture, const glm::vec2& bottomLeftCorner, const glm::vec2& topRightCorner)
            : m_Texture(texture), m_Coords(bottomLeftCorner, topRightCorner) {}

        inline const TexCoords& getTexCoords() const { return m_Coords; }
        inline const SharedPtr<Texture>& getTexture() const { return m_Texture; }
    private:
        TexCoords m_Coords;
        SharedPtr<Texture> m_Texture;
    };

}
