#include "ruby_pch.h"

#include "Ruby/Main/Logging.h"

namespace Ruby 
{
    SharedPtr<Logger> Logger::s_EngineLogger;
    SharedPtr<Logger> Logger::s_ClientLogger;

    static HANDLE s_ConsoleHandle;
    static LogFullColor s_DefaultColor, s_CurrentColor;
    static uint16_t s_DefaultTextAttribs;

    inline LogFullColor LogColorFromWinStyle(uint8_t color)
    {
        LogFullColor res;
        res.FG = (LogColor)((color & 0x0F) + 1);
        res.BG = (LogColor)(((color & 0xF0) >> 4) + 1);
        return res;
    }

    inline uint16_t TextAttribFromColor(LogFullColor color)
    {
        uint16_t res = s_DefaultTextAttribs;
        res |= ((uint8_t)color.BG - 1) << 4;
        return res | ((uint8_t)color.FG - 1);
    }

    void Logger::init()
    {
        s_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(s_ConsoleHandle, &info);

        // Gets the first byte of data in the wAttributes which holds the color info. 
        // This contains the current text color in the first 4 bits and the highlight color in the last 4.
        s_DefaultColor = LogColorFromWinStyle(info.wAttributes & 0xFF);
        s_CurrentColor = s_DefaultColor;
        s_DefaultTextAttribs = info.wAttributes & 0xFF00; 
        s_EngineLogger = createShared<Logger>("Engine", LogLevel::Trace);
        s_ClientLogger = createShared<Logger>("Client", LogLevel::Trace);

    }

    Logger::Logger(const char* name, LogLevel logLevel)
        : m_Name(name), m_Level(logLevel), m_Style{s_DefaultColor}
    {}

    void Logger::resetDefaultLogColor()
    {
        m_Style.Color = s_DefaultColor;
    }

    inline void changeColor(LogFullColor color)
    {
        if (s_CurrentColor.Full != color.Full)
        {
            if (color.BG == LogColor::None)
                color.BG = s_DefaultColor.BG;
            if (color.FG == LogColor::None)
                color.FG = s_DefaultColor.FG;

            SetConsoleTextAttribute(s_ConsoleHandle, TextAttribFromColor(color));
            s_CurrentColor = color;
        }
    }

    inline void resetColor()
    {
        SetConsoleTextAttribute(s_ConsoleHandle, TextAttribFromColor(s_DefaultColor));
        s_CurrentColor = s_DefaultColor;
    }

    void logBase(const char* name, const char*__restrict message, va_list args)
    {
        TimeStruct time = Time::getLocalTime();
        printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, name);
        vprintf(message, args); 
        // If the background is the default, we dont need to reset it because
        // the newline character wont show a different colored background
        if(s_CurrentColor.BG != s_DefaultColor.BG)
            resetColor();
        printf("\n");
    }

    void Logger::basicLog(const char*__restrict message, ...)
    {
        changeColor(s_CurrentColor);

        va_list args;
        va_start(args, message);
        logBase(m_Name, message, args);
        va_end(args);
    }


    void Logger::trace(const char*__restrict message, ...)
    {
        if (getLogLevel() != LogLevel::Trace)
            return;

        changeColor(Logger::TRACE_COLOR);

        va_list args;
        va_start(args, message);
        logBase(m_Name, message, args);
        va_end(args);
    }


    void Logger::info(const char*__restrict message, ...)
    {
        if (getLogLevel() > LogLevel::Info)
            return;

        changeColor(Logger::INFO_COLOR);

        va_list args;
        va_start(args, message);
        logBase(m_Name, message, args);
        va_end(args);
    }


    void Logger::warn(const char*__restrict message, ...)
    {
        if (getLogLevel() > LogLevel::Warn)
            return;

        changeColor(Logger::WARN_COLOR);

        va_list args;
        va_start(args, message);
        logBase(m_Name, message, args);
        va_end(args);
    }


    void Logger::error(const char*__restrict message, ...)
    {
        if (getLogLevel() == LogLevel::Critical)
            return;

        changeColor(Logger::ERROR_COLOR);

        va_list args;
        va_start(args, message);
        logBase(m_Name, message, args);
        va_end(args);
    }


    void Logger::critical(const char*__restrict message, ...)
    {
        changeColor(Logger::CRITICAL_COLOR);

        va_list args;
        va_start(args, message);
        logBase(m_Name, message, args);
        va_end(args);
    }
}
