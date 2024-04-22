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

		virtual void setVertexBuffer(const SharedPtr<VertexBuffer>& buffer) override;
		virtual void setIndexBuffer(const SharedPtr<IndexBuffer>& buffer) override;

		virtual inline const SharedPtr<VertexBuffer>& getVertexBuffer() const override { return m_VertexBuffer; }
		virtual inline const SharedPtr<IndexBuffer>& getIndexBuffer() const override { return m_IndexBuffer; }

	private:
		RendererID m_RendererID = 0;
		SharedPtr<VertexBuffer> m_VertexBuffer{nullptr};
		SharedPtr<IndexBuffer> m_IndexBuffer{nullptr};
	};

}