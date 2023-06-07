#pragma once

#include "Ruby/Render/Buffers.h"

namespace Ruby {

	class OpenGLVB : public VertexBuffer
	{
	public:
		OpenGLVB(const void* vertices, uint32_t size);
		OpenGLVB(uint32_t size);

		virtual ~OpenGLVB();

		virtual void bind() const override;
		virtual void unbind() const override;
		virtual void setVertexData(const void* vertices, uint32_t size, uint32_t offset) override;
		virtual inline uint32_t getSize() { return m_Size; }

		virtual inline void setLayout(const VertexLayout& layout) override { m_Layout = layout; }
		virtual inline const VertexLayout& getLayout() const override { return m_Layout; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Size;
		VertexLayout m_Layout;
	};

	class OpenGLIB : public IndexBuffer
	{
	public:
		OpenGLIB(const uint32_t* indices, uint32_t count);

		virtual ~OpenGLIB();

		virtual void bind() const override;
		virtual void unbind() const override;
		virtual inline uint32_t getCount() const { return m_Count; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Count;
	};

}