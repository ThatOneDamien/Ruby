#pragma once

#include <cstdio>
#include "Ruby/Utility/Pointers.h"
#include "Ruby/Main/Time.h"

namespace Ruby 
{
	enum class LogLevel { Trace = 0, Info, Warn, Error, Critical };

#ifdef RB_PLAT_WIND
	enum class LogColor : uint8_t
	{
		Black = 0,
		Dark_Blue,
		Dark_Green,
		Dark_Cyan,
		Dark_Red,
		Dark_Purple,
		Gold,
		Light_Gray,
		Gray,
		Blue,
		Green,
		Cyan,
		Red,
		Purple,
		Yellow,
		White
	};
	#define COLOR_COMBINE(textColor, highlightColor) (highlightColor << 4) | textColor 
	#define TRACE_COLOR COLOR_COMBINE((uint16_t)LogColor::Green, (uint16_t)LogColor::Black)
	#define INFO_COLOR COLOR_COMBINE((uint16_t)LogColor::Purple, (uint16_t)LogColor::Black)
	#define WARN_COLOR COLOR_COMBINE((uint16_t)LogColor::Yellow, (uint16_t)LogColor::Black)
	#define ERROR_COLOR COLOR_COMBINE((uint16_t)LogColor::Red, (uint16_t)LogColor::Black)
	#define CRITICAL_COLOR COLOR_COMBINE((uint16_t)LogColor::White, (uint16_t)LogColor::Dark_Red)
#elif RB_PLAT_LINUX
	enum class LogColor : uint8_t
	{
		Black = 0, Gray = 0, Light_Gray = 0,
		Red = 1, Dark_Red = 1,
		Green = 2, Dark_Green = 2,
 		Yellow = 3, Gold = 3,
		Blue = 4, Dark_Blue = 4,
		Purple = 5, Dark_Purple = 5,
		Cyan = 6, Dark_Cyan = 6,
		White = 7
	};
	#define TRACE_COLOR (uint16_t)LogColor::Green
	#define INFO_COLOR (uint16_t)LogColor::Purple
	#define WARN_COLOR (uint16_t)LogColor::Yellow
	#define ERROR_COLOR (uint16_t)LogColor::Red
	#define CRITICAL_COLOR (uint16_t)LogColor::Red
#endif

	class Logger
	{
	public:

		Logger(const char* name, LogLevel logLevel = LogLevel::Trace);
		~Logger() = default;
		
		inline void setLogPrefix(const char* name) { m_Name = name;   }
		inline void setLogLevel(LogLevel level)    { m_Level = level; }

		void setLogColor(LogColor textColor, LogColor highlightColor);
		void setLogTextColor(LogColor textColor);
		void setLogHighlightColor(LogColor highlightColor);

		inline const char* getLogName()           { return m_Name;  }
		inline LogLevel getLogLevel()             { return m_Level; }

		void resetDefaultLogColor();

		static void init();
		static SharedPtr<Logger>& getEngineLogger() { return s_EngineLogger; }
		static SharedPtr<Logger>& getClientLogger() { return s_ClientLogger; }




		template<typename... Args>
		inline void basicLog(const char* message, Args... args)
		{
			TimeStruct time = Time::getLocalTime();
			internalSetLogColor(m_SavedColor);
			printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, m_Name);
			printf(message, std::forward<Args>(args)...);
			internalResetLogColor();
			printf("\n");
		}


		template<typename... Args>
		inline void trace(const char* message, Args... args)
		{
			if (getLogLevel() != LogLevel::Trace)
				return;

			internalSetLogColor(TRACE_COLOR);
			TimeStruct time = Time::getLocalTime();
			printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, m_Name);
			printf(message, std::forward<Args>(args)...);
			printf("\n");
		}

		template<typename... Args>
		inline void info(const char* message, Args... args)
		{
			if (getLogLevel() > LogLevel::Info)
				return;

			internalSetLogColor(INFO_COLOR);
			TimeStruct time = Time::getLocalTime();
			printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, m_Name);
			printf(message, args...);
			printf("\n");
		}

		template<typename... Args>
		inline void warn(const char* message, Args... args)
		{
			if (getLogLevel() > LogLevel::Warn)
				return;

			internalSetLogColor(WARN_COLOR);
			TimeStruct time = Time::getLocalTime();
			printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, m_Name);
			printf(message, std::forward<Args>(args)...);
			printf("\n");
		}

		template<typename... Args>
		inline void error(const char* message, Args... args)
		{
			if (getLogLevel() == LogLevel::Critical)
				return;

			internalSetLogColor(ERROR_COLOR);
			TimeStruct time = Time::getLocalTime();
			printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, m_Name);
			printf(message, std::forward<Args>(args)...);
			printf("\n");
		}

		template<typename... Args>
		inline void critical(const char* message, Args... args)
		{
			internalSetLogColor(CRITICAL_COLOR);
			TimeStruct time = Time::getLocalTime();
			printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, m_Name);
			printf(message, std::forward<Args>(args)...);
			printf("\n");
		}


	private:
		void internalSetLogColor(uint16_t color);
		void internalResetLogColor();

		uint16_t m_SavedColor;
		LogLevel m_Level;
		const char* m_Name; // Aka the Log Prefix.
		static SharedPtr<Logger> s_EngineLogger;
		static SharedPtr<Logger> s_ClientLogger;
	};
	#undef TRACE_COLOR
	#undef INFO_COLOR
	#undef WARN_COLOR
	#undef ERROR_COLOR
	#undef CRITICAL_COLOR
}