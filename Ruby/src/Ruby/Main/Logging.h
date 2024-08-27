#pragma once

#include <cstdarg>
#include <cstdio>
#include <cstdint>
#include "Platform.h"
#include "Ruby/Utility/Pointers.h"
#include "Ruby/Main/Time.h"

namespace Ruby 
{
    enum class LogLevel { Trace = 0, Info, Warn, Error, Critical };

#ifdef RB_PLAT_WIND
    enum class LogColor : uint8_t
    {
        None = 0
        Black,
        DarkBlue,
        DarkGreen,
        DarkCyan,
        DarkRed,
        DarkPurple,
        Gold,
        LightGray,
        Gray,
        Blue,
        Green,
        Cyan,
        Red,
        Purple,
        Yellow,
        White,
    };
#elif defined(RB_PLAT_LINUX)
    enum class LogColor : uint8_t
    {
        None = 0,
        Black,
        DarkRed,
        DarkGreen,
        Gold,
        DarkBlue,
        DarkPurple,
        DarkCyan,
        LightGray,
        Gray,
        Red,
        Green,
        Yellow,
        Blue,
        Purple,
        Cyan,
        White
    };
#endif

    union LogFullColor
    {
        uint16_t Full;
        struct
        {
            LogColor FG;
            LogColor BG;
        };
    };

    struct LogStyle
    {
        LogFullColor Color;
        // TODO: Add later
        // bool Underlined;
        // bool Bold;
    };

    class Logger
    {
    public:

        Logger(const char* name, LogLevel logLevel = LogLevel::Trace);
        ~Logger() = default;
        
        inline void setLogPrefix(const char* name) { m_Name = name;   }
        inline void setLogLevel(LogLevel level)    { m_Level = level; }

        void resetDefaultLogColor();
        inline void setLogFG(LogColor foregroundColor) { m_Style.Color.FG = foregroundColor; }
        inline void setLogBG(LogColor backgroundColor) { m_Style.Color.BG = backgroundColor; }
        inline void setLogColor(LogColor foregroundColor, LogColor backgroundColor)
        {
            setLogFG(foregroundColor);
            setLogBG(backgroundColor);
        }

        inline const char* getLogName() { return m_Name;  }
        inline LogLevel getLogLevel()   { return m_Level; }


        // Logging functions

        // Log with currently set log color. This is printed 
        // no matter the log level.
        void basicLog(const char*__restrict message, ...);
        // Log a trace(lowest priority) message.
        void trace(const char*__restrict message, ...);
        // Log an info message.
        void info(const char*__restrict message, ...);
        // Log a warning message.
        void warn(const char*__restrict message, ...);
        // Log an error message
        void error(const char*__restrict message, ...);
        // Log a critical(highest priority) message.
        void critical(const char*__restrict message, ...);




        static void init();
        static SharedPtr<Logger>& getEngineLogger() { return s_EngineLogger; }
        static SharedPtr<Logger>& getClientLogger() { return s_ClientLogger; }


private:
        static SharedPtr<Logger> s_EngineLogger;
        static SharedPtr<Logger> s_ClientLogger;
        const char* m_Name; // Used to prefix the log message.
        LogLevel m_Level;
        LogStyle m_Style;


public:
        static constexpr LogFullColor TRACE_COLOR    = { .FG = LogColor::Green,  .BG = LogColor::None };
        static constexpr LogFullColor INFO_COLOR     = { .FG = LogColor::Purple, .BG = LogColor::None };
        static constexpr LogFullColor WARN_COLOR     = { .FG = LogColor::Gold,   .BG = LogColor::None };
        static constexpr LogFullColor ERROR_COLOR    = { .FG = LogColor::Red,    .BG = LogColor::None };
        static constexpr LogFullColor CRITICAL_COLOR = { .FG = LogColor::Black,  .BG = LogColor::DarkRed };
    };
}
