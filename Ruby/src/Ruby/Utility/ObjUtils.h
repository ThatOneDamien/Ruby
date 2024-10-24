#pragma once

#include "Ruby/Render/Buffers.h"
#include "Ruby/Render/Mesh.h"

#include <glm/glm.hpp>

#include <unordered_map>

namespace Ruby
{
    struct ObjVertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 UV;
        int MatIndex;
    };

    struct MatLib
    {
        std::unordered_map<std::string, size_t> NameToIndex;
        std::vector<Material> Materials;
    };

    struct ObjFilePayload
    {
        std::vector<ObjVertex> Vertices;
        std::vector<uint32_t> Indices;
        MatLib Materials;
    };

    namespace ObjUtils
    {
        ObjFilePayload readObjFile(const std::string& path);
    }

}
