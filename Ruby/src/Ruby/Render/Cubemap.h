#pragma once

#include "Ruby/Main/Core.h"
#include "Ruby/Render/Texture.h"

namespace Ruby
{
    struct CubemapTexPaths
    {
        std::string Right;
        std::string Left;
        std::string Top;
        std::string Bottom;
        std::string Back;
        std::string Front;
    };

    class RB_NOVTABLE Cubemap
    {
    public:
        virtual void bind() const = 0;
        static SharedPtr<Cubemap> create(const CubemapTexPaths& paths);
    };
}
