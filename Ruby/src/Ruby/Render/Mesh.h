#pragma once

#include "Buffers.h"
#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Ruby
{
    struct alignas(16) Material
    {
        glm::vec4 Ambient;
        glm::vec4 Diffuse;
        glm::vec4 Specular;
        float LightModel;
    };

    class Mesh
    {
    public:
        Mesh(const std::string& filepath);

        inline const SharedPtr<VertexArray>& getVAO() const { return m_VAO; }
        inline const std::vector<Material>& getMaterials() const { return m_Materials; }
    private:
        SharedPtr<VertexArray> m_VAO;
        std::vector<Material> m_Materials;
    };
}
