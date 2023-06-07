#pragma once

#include "Ruby/Render/VertexArray.h"

namespace Ruby {

	class OpenGLVA : public VertexArray
	{
	public:
		OpenGLVA();
		virtual ~OpenGLVA();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void pushVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) override;
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) override;

		virtual inline const std::vector<std::shared_ptr<VertexBuffer> >& getVertexBufferList() const override { return m_VertexBuffers; }
		virtual inline const std::shared_ptr<IndexBuffer>& getIndexBuffer() const override { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}