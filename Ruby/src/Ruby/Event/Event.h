#pragma once

#include "Ruby/Main/Core.h"
#include <sstream>

namespace Ruby 
{
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
		virtual inline EventType getType() const { return EventType::NONE; }
		virtual inline std::string toStr() const { return std::string("Empty Event"); }
	};

}