#pragma once

#include "Texture.h"

#include <glm/glm.hpp>
#include <unordered_map>

namespace Ruby 
{
    struct FontData;

    class Font
    {
    public:
        Font(const std::string& filepath);
        ~Font();

        inline const SharedPtr<Texture>& getAtlasTexture() const { return m_AtlasTexture; }
        inline const FontData* getAtlasData() const { return m_Data; }

        static void init();
        static void deInit();
    private:

    private:
        SharedPtr<Texture> m_AtlasTexture;
        FontData* m_Data{nullptr};
    };

}