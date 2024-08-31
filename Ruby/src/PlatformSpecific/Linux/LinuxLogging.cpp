#include "ruby_pch.h"

#include "Ruby/Main/Logging.h"

namespace Ruby 
{
    SharedPtr<Logger> Logger::s_EngineLogger;
    SharedPtr<Logger> Logger::s_ClientLogger;

    static char s_FGColorBuf[6] = "\e[39m";
    static char s_BGColorBuf[7] = "\e[49m\0"; 

    static LogFullColor s_CurrentColor;

    void Logger::init()
    {
        s_EngineLogger = createShared<Logger>("Engine", LogLevel::Trace);
        s_ClientLogger = createShared<Logger>("Client", LogLevel::Trace);
    }

    Logger::Logger(const char* name, LogLevel logLevel)
        : m_Name(name), m_Level(logLevel)
    {}

    void Logger::resetDefaultLogColor()
    {
        m_Style.Color.Full = (uint16_t)LogColor::None;
    }

    static void changeColor(LogFullColor color)
    {
        if(color.FG != s_CurrentColor.FG)
        {
            bool isNone = color.FG == LogColor::None;
            bool isDark = (uint8_t)color.FG < 9;
            s_FGColorBuf[2] = isDark ? '3' : '9';
            s_FGColorBuf[3] = isNone ? '9' : '0' + (((uint8_t)color.FG - 1) & 7);
            s_CurrentColor.FG = color.FG;
        }
        printf(s_FGColorBuf);

        if(color.BG != s_CurrentColor.BG)
        {
            bool isNone = color.BG == LogColor::None;
            bool isDark = (uint8_t)color.BG < 9;
            char effective = '0' + (((uint8_t)color.BG - 1) & 7);
            s_BGColorBuf[2] = isDark ? '4' : '1';
            s_BGColorBuf[3] = isNone ? '9' : (isDark ? effective : '0');
            s_BGColorBuf[4] = isDark ? 'm' : effective; 
            s_BGColorBuf[5] = isDark ? '\0' : 'm';
            s_CurrentColor.BG = color.BG;
        }
        printf(s_BGColorBuf);
    }

    void logBase(const char* name, const char*__restrict message, va_list args)
    {
        TimeStruct time = Time::getLocalTime();
        printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, name);
        vprintf(message, args); 
        printf("\e[0m\n");
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
