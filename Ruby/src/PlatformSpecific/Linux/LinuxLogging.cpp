#include "ruby_pch.h"

#include "Ruby/Main/Core.h"

#include "Ruby/Main/Logging.h"

namespace Ruby 
{
    SharedPtr<Logger> Logger::s_EngineLogger;
    SharedPtr<Logger> Logger::s_ClientLogger;
    constexpr const char* COLOR_CONVERSIONS[8] = { "\e[30m", "\e[31m", "\e[32m", "\e[33m", "\e[34m", "\e[35m", "\e[36m", "\e[37m" };

    void Logger::init()
    {

        s_EngineLogger = createShared<Logger>("Engine", LogLevel::Trace);
        s_ClientLogger = createShared<Logger>("Client", LogLevel::Trace);

    }

    Logger::Logger(const char* name, LogLevel logLevel)
        : m_Name(name), m_Level(logLevel), m_SavedColor((uint16_t)LogColor::White)
    {}

    void Logger::setLogColor(LogColor textColor, LogColor highlightColor)
    {
        (void)highlightColor;
        setLogTextColor(textColor);
    }

    void Logger::setLogTextColor(LogColor textColor)
    {
        m_SavedColor = (uint16_t)textColor;
    }

    void Logger::setLogHighlightColor(LogColor highlightColor) {(void)highlightColor;}

    void Logger::resetDefaultLogColor()
    {
        m_SavedColor = (uint16_t)LogColor::White;
    }

    void Logger::internalSetLogColor(uint16_t color)
    {
        printf(COLOR_CONVERSIONS[color]);
    }

    void Logger::internalResetLogColor()
    {
        printf("\e[0m");
    }
}
