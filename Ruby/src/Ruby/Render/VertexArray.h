#pragma once

#include "Buffers.h"

namespace Ruby 
{
    class RB_NOVTABLE VertexArray
    {
    public:
        virtual ~VertexArray() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        
        virtual void setVertexBuffer(const SharedPtr<VertexBuffer>& buffer) = 0;
        virtual void setIndexBuffer(const SharedPtr<IndexBuffer>& buffer) = 0;
        
        virtual inline const SharedPtr<VertexBuffer>& getVertexBuffer() const = 0;
        virtual inline const SharedPtr<IndexBuffer>& getIndexBuffer() const = 0;

        static SharedPtr<VertexArray> createVAO();


    };

}