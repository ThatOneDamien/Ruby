#include "ruby_pch.h"

#include "Mesh.h"
#include "Ruby/Utility/ObjUtils.h"

#include <glm/glm.hpp>

namespace Ruby
{
    Mesh::Mesh(const std::string& filepath)
    {
        struct MeshVertex
        {
            glm::vec3 Position;
            glm::vec3 Normal;
            glm::vec2 UV;
            float MatIndex;
        };

        MeshVertex* data = nullptr;
        size_t vertexCnt = 0;
        std::vector<uint32_t> indices;

        {
            ObjFilePayload res = ObjUtils::readObjFile(filepath);
            RB_ENSURE(res.Vertices.size(), "Mesh contains 0 vertices, check the file.");
            indices = std::move(res.Indices);
            vertexCnt = res.Vertices.size();
            data = new MeshVertex[vertexCnt];

            for(size_t i = 0; i < vertexCnt; ++i)
            {
                data[i].Position = res.Vertices[i].Position;
                data[i].Normal = res.Vertices[i].Normal;
                data[i].UV = res.Vertices[i].UV;
                data[i].MatIndex = (float)res.Vertices[i].MatIndex;
            }
            m_Materials = std::move(res.Materials.Materials);
        }

        VertexLayout layout;
        layout.push(LayoutType::Float, 3, false);
        layout.push(LayoutType::Float, 3, false);
        layout.push(LayoutType::Float, 2, false);
        layout.push(LayoutType::Float, 1, false);
        SharedPtr<VertexBuffer> vbo = VertexBuffer::create(data, sizeof(MeshVertex) * vertexCnt);
        vbo->setLayout(layout);
        SharedPtr<IndexBuffer> ibo = IndexBuffer::create(indices.data(), indices.size());

        m_VAO = VertexArray::create();

        m_VAO->setIndexBuffer(ibo);
        m_VAO->setVertexBuffer(vbo);
        delete[] data;
    }
}
