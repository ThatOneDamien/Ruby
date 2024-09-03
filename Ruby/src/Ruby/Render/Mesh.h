#pragma once

#include "Buffers.h"
#include "VertexArray.h"
#include "Ruby/Utility/ObjUtils.h"

namespace Ruby
{
    class Mesh
    {
    public:
        Mesh(const std::string& filepath);

        inline const SharedPtr<VertexArray>& getVAO() const { return m_VAO; }
    private:
        SharedPtr<VertexArray> m_VAO;
    };
}
