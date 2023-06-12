#pragma once

namespace Ruby {

	class Window
	{
	public:
		virtual ~Window() {};
		virtual inline void* getWinPointer() const = 0;
		virtual inline uint16_t getHeight() const = 0;
		virtual inline uint16_t getWidth() const = 0;
		virtual inline bool isVSynced() const = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual void update() const = 0;

		static std::unique_ptr<Window> createWindow(
			const char* name = "Ruby", 
			uint16_t width = 1280, 
			uint16_t height = 720, 
			bool vSync = true
		);

	};

}