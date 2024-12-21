#pragma once

#include "Buffers.h"
#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Ruby
{
    struct alignas(16) Material
    {
        glm::vec4 Ambient  = { 0.0f, 0.0f, 0.0f, 0.0f };
        glm::vec4 Diffuse  = { 0.0f, 0.0f, 0.0f, 0.0f };
        glm::vec4 Specular = { 0.0f, 0.0f, 0.0f, 0.0f };
        float SpecularHl   = 1.0f;
        float LightModel   = 0.0f;
    };

    class Mesh
    {
    public:
        Mesh(const std::string& filepath);
        Mesh(const SharedPtr<VertexArray>& vao, const std::vector<Material>& materials)
            : m_VAO(vao), m_Materials(materials) {}

        inline const SharedPtr<VertexArray>& getVAO() const { return m_VAO; }
        inline const std::vector<Material>& getMaterials() const { return m_Materials; }
    private:
        SharedPtr<VertexArray> m_VAO;
        std::vector<Material> m_Materials;
    };
}
