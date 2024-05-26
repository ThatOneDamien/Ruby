#pragma once

#include "Ruby/Main/Core.h"
#include "Ruby/Render/Buffers.h"

namespace Ruby 
{
	// OpenGL Vertex Buffer
	class OpenGLVB : public VertexBuffer
	{
	public:
		OpenGLVB(const void* vertices, uint32_t size);
		OpenGLVB(uint32_t size);

		virtual ~OpenGLVB();

		virtual void bind() const override;
		virtual void unbind() const override;
		virtual void setVertexData(const void* vertices, uint32_t size, uint32_t offset) override;
		virtual inline uint32_t getSize() const override { return m_Size; }

		virtual inline void setLayout(const VertexLayout& layout) override { m_Layout = layout; }
		virtual inline const VertexLayout& getLayout() const override { return m_Layout; }

	private:
		RendererID m_RendererID = 0;
		uint32_t m_Size;
		VertexLayout m_Layout;
	};

	// OpenGL Index Buffer
	class OpenGLIB : public IndexBuffer
	{
	public:
		OpenGLIB(const uint32_t* indices, uint32_t count);

		virtual ~OpenGLIB();

		virtual void bind() const override;
		virtual void unbind() const override;
		virtual inline uint32_t getCount() const override { return m_Count; }

	private:
		RendererID m_RendererID = 0;
		uint32_t m_Count;
	};

	// OpenGL Uniform Buffer
	class OpenGLUB : public UniformBuffer
	{
	public:
		OpenGLUB(const void* data, uint32_t size, uint8_t binding);
		OpenGLUB(uint32_t size, uint8_t binding);

		virtual ~OpenGLUB();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void setData(const void* data, uint32_t size, uint32_t offset) override;
		virtual inline uint32_t getSize() const override { return m_Size; }
		virtual inline uint8_t getBinding() const override { return m_Binding; }

	private:
		RendererID m_RendererID = 0;
		uint32_t m_Size;
		uint8_t m_Binding;
	};

}