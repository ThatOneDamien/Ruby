#pragma once

#include "Buffers.h"

namespace Ruby {

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		
		virtual void pushVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) = 0;
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) = 0;

		virtual inline const std::vector<std::shared_ptr<VertexBuffer> >& getVertexBufferList() const = 0;
		virtual inline const std::shared_ptr<IndexBuffer>& getIndexBuffer() const = 0;

		static std::shared_ptr<VertexArray> createVAO();


	};

}