#pragma once

#include "Ruby/Utility/Pointers.h"

#include <vector>

namespace Ruby 
{

    enum class LayoutType : uint8_t
    {
        Byte      = 0b00000001,
        UByte     = 0b00010001,
        Short     = 0b00100010,
        UShort    = 0b00110010,
        Int       = 0b01000100,
        UInt      = 0b01010100,
        Float     = 0b01100100,
        Double    = 0b10101000
    };

    inline uint8_t sizeOfLayoutType(LayoutType type) { return (uint8_t)type & 0xF; }

    struct VertexLayoutElement
    {
        LayoutType type;
        uint8_t count;
        bool normalize;
    };

    class VertexLayout
    {
    public:
        VertexLayout() = default;
        ~VertexLayout() = default;
        inline void push(LayoutType type, uint8_t count, bool normalize) 
        { 
            m_Elements.push_back({ type, count, normalize });
            m_Stride += sizeOfLayoutType(type) * count;
        }

        inline void push(VertexLayoutElement&& element)
        {
            m_Elements.push_back(element);
            m_Stride += sizeOfLayoutType(element.type) * element.count;
        }

        inline void push(const VertexLayoutElement& element)
        {
            m_Elements.push_back(element);
            m_Stride += sizeOfLayoutType(element.type) * element.count;
        }


        inline const std::vector<VertexLayoutElement>& getElements() const { return m_Elements; }
        inline uint32_t getStride() const { return m_Stride; }
    private:
        std::vector<VertexLayoutElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    class RB_NOVTABLE VertexBuffer 
    {
    public:
        virtual ~VertexBuffer() {}

        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual void setVertexData(const void* vertices, uint32_t size, uint32_t offset) = 0;
        virtual inline uint32_t getSize() const = 0;

        virtual inline void setLayout(const VertexLayout& layout) = 0;
        virtual inline const VertexLayout& getLayout() const = 0;

        static SharedPtr<VertexBuffer> create(const void* vertices, uint32_t size);
        static SharedPtr<VertexBuffer> create(uint32_t size);

    };

    class RB_NOVTABLE IndexBuffer
    {
    public:
        virtual ~IndexBuffer() {}

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual inline uint32_t getCount() const = 0;

        static SharedPtr<IndexBuffer> create(const uint32_t* indices, uint32_t count);

    };

    class RB_NOVTABLE UniformBuffer
    {
    public:
        virtual ~UniformBuffer() {}

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void setData(const void* data, uint32_t size, uint32_t offset) = 0;
        virtual inline uint32_t getSize() const = 0;
        virtual inline uint8_t getBinding() const = 0;

        static SharedPtr<UniformBuffer> create(const void* data, uint32_t size, uint8_t binding);
        static SharedPtr<UniformBuffer> create(uint32_t size, uint8_t binding);
    };

}
