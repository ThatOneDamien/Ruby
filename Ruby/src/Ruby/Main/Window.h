#pragma once

#include "Core.h"

#include "MouseCodes.h"
#include "KeyCodes.h"

namespace Ruby {

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

	class RB_NOVTABLE Window
	{
	public:
		virtual ~Window() {};
		virtual inline void* getWinPointer() const = 0;
		virtual inline uint16_t getHeight() const = 0;
		virtual inline uint16_t getWidth() const = 0;
		virtual inline float getAspectRatio() const = 0;
		virtual inline void windowResized(uint16_t new_width, uint16_t new_height) = 0;
		virtual inline bool isVSynced() const = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual void update() const = 0;

		virtual bool isKeyDown(KeyCode code) const = 0;
		virtual bool isMouseButtonDown(MouseCode code) const = 0;

		static UniPtr<Window> createWindow(const WindowSpec& spec);

	};

}