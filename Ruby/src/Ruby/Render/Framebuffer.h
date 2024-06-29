#pragma once

#include "Ruby/Utility/Pointers.h"

namespace Ruby 
{
    class RB_NOVTABLE Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual inline uint32_t getTextureID() const = 0;

        virtual void regenerate(int width, int height) = 0;

        static SharedPtr<Framebuffer> create(int width, int height);

    };

}