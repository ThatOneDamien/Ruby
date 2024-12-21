#pragma once

#include "Cubemap.h"
#include "Ruby/Scene/Scene.h"
#include "Mesh.h"


#include <glm/glm.hpp>

namespace Ruby 
{

    namespace Renderer3D 
    {
        // Clears any previous data, allocates new storage, and initializes 
        // the 3D renderer to its base state.
        void init();
        void reload();

        // Deallocates all data owned by the renderer, and
        // sets it to a blank state. By calling this manually,
        // you effectively remove batch rendering capabilities.
        void clear();


        // Use provided Camera's projection matrix in the GPU for
        // projection of the batched geometry.
        void useCamera(const PerspCamera& cam);

        void useCubemap(const SharedPtr<Cubemap>& cubemap);


        void setMesh(const Mesh& mesh);
        void addInstance(const glm::vec3& position, const glm::vec3& scale, float rotation, const glm::vec3& rotationalAxis);
        void renderInstanced();
        void renderCubemap();

    }

}
