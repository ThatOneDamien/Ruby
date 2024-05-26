#pragma once

#include "Event.h"
#include "Ruby/Main/MouseCodes.h"

namespace Ruby 
{
	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(uint16_t X, uint16_t Y) : m_X(X), m_Y(Y) {}

		inline uint16_t getX() const { return m_X; }
		inline uint16_t getY() const { return m_Y; }
		inline std::pair<uint16_t, uint16_t> getCoords() const { return {m_X, m_Y}; }
		
		virtual inline EventType getType() const override { return EventType::MouseMove; }
		virtual inline std::string toStr() const override
		{
			std::stringstream ss;
			ss << "Mouse Moved (x: " << m_X << ", y: " << m_Y << ')';
			return ss.str();
		}
	
	private:
		uint16_t m_X, m_Y;
	};

	class MousePressedEvent : public Event
	{
	public:
		MousePressedEvent(MouseCode buttonCode) : m_ButtonCode(buttonCode) {}

		inline MouseCode getButtonCode() const { return m_ButtonCode; }
		
		virtual inline EventType getType() const override { return EventType::MousePressed; }
		virtual inline std::string toStr() const override 
		{
			std::stringstream ss;
			ss << "Mouse Button Pressed " << (uint16_t)m_ButtonCode;
			return ss.str();
		}

	private:
		MouseCode m_ButtonCode;
	};

	class MouseReleasedEvent : public Event
	{
	public:
		MouseReleasedEvent(MouseCode buttonCode) : m_ButtonCode(buttonCode) {}
		inline MouseCode getKeyCode() const { return m_ButtonCode; }
		
		virtual inline EventType getType() const override { return EventType::MouseReleased; }
		virtual inline std::string toStr() const override
		{
			std::stringstream ss;
			ss << "Mouse Button Released " << (uint16_t)m_ButtonCode;
			return ss.str();
		}

	private:
		MouseCode m_ButtonCode;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float xOff, float yOff) : m_XOff(xOff), m_YOff(yOff) {}

		inline float getXOffset() const { return m_XOff; }
		inline float getYOffset() const { return m_YOff; }
		
		virtual inline EventType getType() const override { return EventType::MouseScroll; }
		virtual inline std::string toStr() const override
		{
			std::stringstream ss;
			ss << "Mouse Button Scrolled (xOff: " << m_XOff << " yOff: " << m_YOff << ')';
			return ss.str();
		}

	private:
		float m_XOff, m_YOff;
	};

}