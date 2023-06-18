#pragma once

#include <glm/glm.hpp>

namespace Ruby {

	class Texture;

	class SubTexture
	{
	public:
		SubTexture(const SharedPtr<Texture>& texture, const glm::vec2& bottomLeftCorner, const glm::vec2& topRightCorner);

		inline const glm::vec2* getTexCoords() const { return m_TexCoords; }
		inline const SharedPtr<Texture>& getTexture() const { return m_Texture; }
		static inline SharedPtr<SubTexture> createSubTexture(const SharedPtr<Texture>& texture, const glm::vec2& bottomLeftCorner, const glm::vec2& topRightCorner)
		{
			return createShared<SubTexture>(texture, bottomLeftCorner, topRightCorner);
		}
	private:
		glm::vec2 m_TexCoords[4];
		SharedPtr<Texture> m_Texture;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;
		
		virtual void bind(uint8_t slot) const = 0;
		virtual void unbind() const = 0;
		virtual void setData(const void* data, uint32_t size) = 0;

		static SharedPtr<Texture> createTexture(const std::string& filepath);
		static SharedPtr<Texture> createTexture(int width, int height);

	};

}
