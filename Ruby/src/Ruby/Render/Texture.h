#pragma once

#include <glm/glm.hpp>

namespace Ruby {

	// The way the numbers are set up are for the purpose of what follows:
	// 
	// By bit shifting these numbers to the right 3 times, the number will
	// be equivalent to the OpenGL internal format defines.
	//
	// Also by using the bitwise and with this number and 7, the result will
	// be the number of bytes per channel this format contains.
	// 
	enum class PixelFormat : uint32_t
	{
		R8    = (0x8229 << 3) + 1,
		RG8   = (0x822B << 3) + 2,
		RGB8  = (0x8051 << 3) + 3,
		RGBA8 = (0x8058 << 3) + 4
	};


	// FROM OPENGL
	enum class TextureFilter
	{
		Nearest = 0x2600,
		Linear  = 0x2601,
	};

	// FROM OPENGL
	enum class TextureWrap
	{
		ClampToEdge       = 0x812F,
		ClampToBorder     = 0x812D,
		MirroredRepeat    = 0x8370,
		Repeat            = 0x2901,
		MirrorClampToEdge = 0x8743
	};

	struct TextureSpec
	{
		int Width;
		int Height;
		PixelFormat Format;
		TextureFilter MinFilter; // Minimizing function for when the texture needs to be rendered at a smaller size.
		TextureFilter MagFilter; // Magnifying function for when the texture needs to be rendered at a larger size.
		TextureWrap WrapS;
		TextureWrap WrapT;
		
		TextureSpec()
			: Width(1), Height(1), Format(PixelFormat::RGBA8),
			MinFilter(TextureFilter::Linear), MagFilter(TextureFilter::Linear),
			WrapS(TextureWrap::ClampToEdge),
			WrapT(TextureWrap::ClampToEdge)
		{}
	};

	class RB_NOVTABLE Texture
	{
	public:
		virtual ~Texture() = default;
		
		virtual void bind(uint8_t slot) const = 0;
		virtual void unbind() const = 0;
		virtual void setData(const void* data, uint32_t size) = 0;

		virtual inline int8_t getBoundSlot() const = 0;
		virtual inline glm::vec2 getSize() const = 0;

		static SharedPtr<Texture> createTexture(const std::string& filepath);
		static SharedPtr<Texture> createTexture(const TextureSpec& spec = TextureSpec());
		static SharedPtr<Texture> createTexture(const void* data, const TextureSpec& spec = TextureSpec());
	};

	class SubTexture
	{
	public:
		SubTexture(const SharedPtr<Texture>& texture, const glm::vec2& bottomLeftCorner, const glm::vec2& topRightCorner);

		inline const glm::vec4& getTexCoords() const { return m_TexCoords; }
		inline const SharedPtr<Texture>& getTexture() const { return m_Texture; }
		static inline SharedPtr<SubTexture> createSubTexture(const SharedPtr<Texture>& texture, const glm::vec2& bottomLeftCorner, const glm::vec2& topRightCorner)
		{
			return createShared<SubTexture>(texture, bottomLeftCorner, topRightCorner);
		}
	private:
		glm::vec4 m_TexCoords;
		SharedPtr<Texture> m_Texture;
	};

}
