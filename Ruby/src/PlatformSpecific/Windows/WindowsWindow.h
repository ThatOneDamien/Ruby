#pragma once

#include "Ruby/Main/Window.h"

struct GLFWwindow;

namespace Ruby {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const char* name, uint16_t width, uint16_t height, bool vSync);
		virtual ~WindowsWindow();
		
		virtual inline void* getWinPointer() const override { return m_Window; }
		virtual inline uint16_t getHeight() const override { return m_Height; }
		virtual inline uint16_t getWidth() const override { return m_Width; }
		virtual inline bool isVSynced() const override { return m_VSync; }
		virtual void setVSync(bool enabled) override;
		virtual void update() const override;

	private:
		GLFWwindow* m_Window = nullptr;
		const char* m_Name;
		uint16_t m_Width, m_Height;
		bool m_VSync;
	};

}