#pragma once

#include "Ruby/Main/Core.h"
#include <sstream>

#define RB_NONE              0x0000
#define RB_EVENT_KEYBOARD    0x0001
#define RB_EVENT_MOUSE       0x0002
#define RB_EVENT_MOUSEBUTTON 0x0004
#define RB_EVENT_INPUT		 0x0008
#define RB_EVENT_APP		 0x0010

namespace Ruby {

	enum class EventType
	{
		NONE = 0,
		MousePressed, MouseReleased, MouseScroll, MouseMove,
		KeyPressed, KeyReleased,
		WindowClose, WindowResized, WindowLostFocus, WindowRegainFocus, WindowMoved
	};

	class Event
	{
	public:
		bool handeled = false;
		//Returns the categories of the event (stored in a bitfield)
		virtual inline uint16_t getCategory() const { return RB_NONE; }
		virtual inline EventType getType() const { return EventType::NONE; }
		inline bool inCategory(uint16_t category_list)
		{
			return getCategory() & category_list;
		}
		virtual inline std::string toStr() const { return std::string("Empty Event"); }
	};

}