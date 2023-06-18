#pragma once

#include "Buffers.h"

namespace Ruby {

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		
		virtual void pushVertexBuffer(const SharedPtr<VertexBuffer>& buffer) = 0;
		virtual void setIndexBuffer(const SharedPtr<IndexBuffer>& buffer) = 0;
		
		virtual inline const std::vector<SharedPtr<VertexBuffer> >& getVertexBufferList() const = 0;
		virtual inline const SharedPtr<IndexBuffer>& getIndexBuffer() const = 0;

		static SharedPtr<VertexArray> createVAO();


	};

}