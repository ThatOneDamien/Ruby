#pragma once

#include "Core.h"

#include "MouseCodes.h"
#include "KeyCodes.h"

// Forward Declaration
struct GLFWwindow;

namespace Ruby 
{
	struct WindowSpec
	{
		const char* Name;
		int Width;
		int Height;
		int MinWidth;
		int MinHeight;
		bool VSync;
		bool HasTitleBar;
	};

	class Window
	{
	public:
		Window() = delete;
		Window(const Window&) = delete;
		Window(const WindowSpec& spec);
		~Window();
		inline void* getWinPointer() const { return m_Window; }
		inline uint16_t getWidth() const { return m_Width; }
		inline uint16_t getHeight() const { return m_Height; }
		inline float getAspectRatio() const { return (float)m_Width / (float)m_Height; }
		inline void windowResized(uint16_t new_width, uint16_t new_height) { m_Width = new_width; m_Height = new_height; }
		inline bool isVSynced() const { return m_VSync; }
		void setVSync(bool enabled);
		void update() const;

		bool isKeyDown(KeyCode code) const;
		bool isMouseButtonDown(MouseCode code) const;
	private:
		GLFWwindow* m_Window = nullptr;
		const char* m_Name;
		uint16_t m_Width, m_Height;
		bool m_VSync;
		bool m_Minimized = false;
	};
}