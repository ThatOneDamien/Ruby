#pragma once

#include "Ruby/Main/Window.h"

struct GLFWwindow;

namespace Ruby {

	class LinuxWindow : public Window
	{
	public:
		LinuxWindow(const char* name, uint16_t width, uint16_t height, bool vSync);
		virtual ~LinuxWindow();
		
		virtual inline void* getWinPointer() const override { return m_Window; }
		virtual inline uint16_t getHeight() const override { return m_Height; }
		virtual inline uint16_t getWidth() const override { return m_Width; }
		virtual inline float getAspectRatio() const override { return (float)m_Width / (float)m_Height; }
		virtual inline void windowResized(uint16_t new_width, uint16_t new_height) { m_Width = new_width; m_Height = new_height; }
		virtual inline bool isVSynced() const override { return m_VSync; }
		virtual void setVSync(bool enabled) override;
		virtual void update() const override;

		virtual bool isKeyDown(KeyCode code) const override;
		virtual bool isMouseButtonDown(MouseCode code) const override;

	private:
		GLFWwindow* m_Window = nullptr;
		const char* m_Name;
		uint16_t m_Width, m_Height;
		bool m_VSync;
		bool m_Minimized = false;
	};

}