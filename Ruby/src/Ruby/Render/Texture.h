#pragma once

namespace Ruby {

	class Texture
	{
	public:
		virtual ~Texture() = default;
		
		virtual void bind(uint8_t slot) const = 0;
		virtual void unbind() const = 0;
		virtual void setData(const void* data, uint32_t size) = 0;

		static std::shared_ptr<Texture> createTexture(const std::string& filepath);
		static std::shared_ptr<Texture> createTexture(int width, int height);
	};

}
