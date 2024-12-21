#pragma once

#include "Ruby/Render/Cubemap.h"

namespace Ruby
{
    class OpenGLCubemap : public Cubemap
    {
    public:
        OpenGLCubemap(const CubemapTexPaths& paths);
        virtual void bind() const override;
    private:
        RendererID m_RendererID;
    };

}
