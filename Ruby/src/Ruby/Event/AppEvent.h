#pragma once

#include "Event.h"

namespace Ruby 
{
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}

        inline uint32_t getHeight() const { return m_Height; }
        inline uint32_t getWidth() const { return m_Width; }
        virtual inline EventType getType() const override { return EventType::WindowResized; }
        virtual inline std::string toStr() const override
        {
            std::stringstream ss;
            ss << "Window Resized: " << m_Width << ", " << m_Height;
            return ss.str();
        }


    private:
        uint32_t m_Width, m_Height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}

        virtual inline EventType getType() const override { return EventType::WindowClose; }
        virtual inline std::string toStr() const override
        {
            std::stringstream ss;
            ss << "Window Close Event Triggered";
            return ss.str();
        }

    };

    class WindowMovedEvent : public Event
    {
    public:
        WindowMovedEvent(uint16_t X, uint16_t Y) : m_X(X), m_Y(Y) {}

        inline uint16_t getX() const { return m_X; }
        inline uint16_t getY() const { return m_Y; }
        inline std::pair<uint16_t, uint16_t> getCoords() const { return {m_X, m_Y}; }
        
        virtual inline EventType getType() const override { return EventType::WindowMoved; }
        virtual inline std::string toStr() const override
        {
            std::stringstream ss;
            ss << "Window Moved: " << m_X << ", " << m_Y;
            return ss.str();
        }

    private:
        uint16_t m_X, m_Y;
    };


}