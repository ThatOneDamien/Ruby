#pragma once

#include "Event.h"
#include "Ruby/Main/KeyCodes.h"

namespace Ruby {

	class KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(KeyCode keyCode, bool repeat) : m_KeyCode(keyCode), m_Repeat(repeat) {}

		inline bool isRepeat() { return m_Repeat; }
		inline KeyCode getKeyCode() const { return m_KeyCode; }
		
		virtual inline EventType getType() const override { return EventType::KeyPressed; }
		virtual inline std::string toStr() const override
		{
			std::stringstream ss;
			ss << "Key " << (m_Repeat ? "Repeated " : "Pressed ") << m_KeyCode;
			return ss.str();
		}


	private:
		KeyCode m_KeyCode;
		bool m_Repeat;
	};

	class KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent(KeyCode keyCode) : m_KeyCode(keyCode) {}

		inline KeyCode getKeyCode() const { return m_KeyCode; }
		
		virtual inline EventType getType() const override { return EventType::KeyReleased; }
		virtual inline std::string toStr() const override
		{
			std::stringstream ss;
			ss << "Key Released " << m_KeyCode;
			return ss.str();
		}

	private:
		KeyCode m_KeyCode;
	};

}