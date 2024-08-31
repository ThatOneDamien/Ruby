#pragma once

#include "Buffers.h"
#include "Camera.h"
#include "Font.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Ruby/Scene/Scene.h"


#include <glm/glm.hpp>

namespace Ruby 
{

    namespace Renderer3D 
    {
        // Clears any previous data, allocates new storage, and initializes 
        // the 3D renderer to its base state.
        void init();

        // Deallocates all data owned by the renderer, and
        // sets it to a blank state. By calling this manually,
        // you effectively remove batch rendering capabilities.
        void clear();


        // Use provided Camera's projection matrix in the GPU for
        // projection of the batched geometry.
        void useCamera(const PerspCamera& cam);

    }

}
