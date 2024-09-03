#pragma once

#include "Ruby/Render/Buffers.h"

#include <glm/glm.hpp>

namespace Ruby
{
    struct ObjVertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 UV;
    };

    struct ObjFilePayload
    {
        std::vector<ObjVertex> Vertices;
        std::vector<uint32_t> Indices;
    };

    namespace ObjUtils
    {
        ObjFilePayload readObjFile(const std::string& path);
    }

}
