#pragma once

#include <cstdio>
#include "Ruby/Utility/Pointers.h"
#include "Ruby/Main/Time.h"

namespace Ruby 
{
	enum class LogLevel { Trace = 0, Info, Warn, Error, Critical };

	// Currently from Windows console colors. TODO abstract this enum so it is not dependent on Windows.
	enum class LogColor : uint8_t
	{
		Black = 0,
		Dark_Blue = 1,
		Dark_Green = 2,
		Dark_Aqua, Dark_Cyan = 3,
		Dark_Red = 4,
		Dark_Purple = 5, Dark_Magenta = 5,
		Gold = 6,
		Light_Gray = 7,
		Gray = 8,
		Blue = 9,
		Green = 10,
		Aqua = 11, Cyan = 11,
		Red = 12,
		Purple = 13, Magenta = 13,
		Yellow = 14,
		White = 15
	};

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

			internalSetLogColor(LogColor::Green, LogColor::Black);
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

			internalSetLogColor(LogColor::Purple, LogColor::Black);
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

			internalSetLogColor(LogColor::Yellow, LogColor::Black);
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

			internalSetLogColor(LogColor::Red, LogColor::Black);
			TimeStruct time = Time::getLocalTime();
			printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, m_Name);
			printf(message, std::forward<Args>(args)...);
			printf("\n");
		}

		template<typename... Args>
		inline void critical(const char* message, Args... args)
		{
			internalSetLogColor(LogColor::White, LogColor::Dark_Red);
			TimeStruct time = Time::getLocalTime();
			printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, m_Name);
			printf(message, std::forward<Args>(args)...);
			internalResetLogColor();
			printf("\n");
		}


	private:
		void internalSetLogColor(LogColor textColor, LogColor highlightColor);
		void internalSetLogColor(uint16_t color);
		void internalResetLogColor();

		uint16_t m_CurrentColor, m_SavedColor;
		LogLevel m_Level;
		const char* m_Name; // Aka the Log Prefix.
		static SharedPtr<Logger> s_EngineLogger;
		static SharedPtr<Logger> s_ClientLogger;
	};

}