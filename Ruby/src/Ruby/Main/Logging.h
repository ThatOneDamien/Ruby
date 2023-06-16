#pragma once

#include <cstdio>
#include <memory>
#include "Ruby/Main/Time.h"

namespace Ruby {

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
		Aqua = 11, cyan = 11,
		Red = 12,
		Purple = 13, Magenta = 13,
		Yellow = 14,
		White = 15
	};

	class Logger
	{
	public:

		Logger(const char* prefix, LogLevel logLevel = LogLevel::Trace);
		~Logger() = default;

		static void init();
		static inline std::shared_ptr<Logger>& getEngineLogger() { return s_EngineLogger; }
		
		inline void setLogPrefix(const char* name) { m_Name = name; }

		void setLogColor(LogColor textColor, LogColor highlightColor);
		void setLogTextColor(LogColor textColor);
		void setLogHighlightColor(LogColor highlightColor);

		inline void setLogLevel(LogLevel level) { m_Level = level; }

		void resetDefaultLogColor();

		inline const char* getLogPrefix() { return m_Name; }
		inline LogLevel getLogLevel() { return m_Level; }


		template<typename ...Args>
		inline void basicLog(const char* message, Args... args)
		{
			TimeStruct time = Time::getLocalTime();
			printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, m_Name);
			printf(message, args...);
			printf("\n");
		}


		template<typename... Args>
		inline void trace(const char* message, Args... args)
		{
			if (getLogLevel() != LogLevel::Trace)
				return;

			setLogColor(LogColor::Green, LogColor::Black);
			TimeStruct time = Time::getLocalTime();
			printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, m_Name);
			printf(message, args...);
			resetDefaultLogColor();
			printf("\n");
		}

		template<typename... Args>
		inline void info(const char* message, Args... args)
		{
			if (getLogLevel() > LogLevel::Info)
				return;

			setLogColor(LogColor::Yellow, LogColor::Black);
			TimeStruct time = Time::getLocalTime();
			printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, m_Name);
			printf(message, args...);
			resetDefaultLogColor();
			printf("\n");
		}

		template<typename... Args>
		inline void warn(const char* message, Args... args)
		{
			if (getLogLevel() > LogLevel::Warn)
				return;

			setLogColor(LogColor::Purple, LogColor::Black);
			TimeStruct time = Time::getLocalTime();
			printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, m_Name);
			printf(message, args...);
			resetDefaultLogColor();
			printf("\n");
		}

		template<typename... Args>
		inline void error(const char* message, Args... args)
		{
			if (getLogLevel() == LogLevel::Critical)
				return;

			setLogColor(LogColor::Red, LogColor::Black);
			TimeStruct time = Time::getLocalTime();
			printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, m_Name);
			printf(message, args...);
			resetDefaultLogColor();
			printf("\n");
		}

		template<typename... Args>
		inline void critical(const char* message, Args... args)
		{
			setLogColor(LogColor::White, LogColor::Dark_Red);
			TimeStruct time = Time::getLocalTime();
			printf("[%.2d:%.2d:%.2d] %s: ", time.hour, time.minute, time.second, m_Name);
			printf(message, args...);
			resetDefaultLogColor();
			printf("\n");
		}


	private:
		uint16_t m_ConsoleColor;
		LogLevel m_Level;
		const char* m_Name; // Aka the Log Prefix.
		static std::shared_ptr<Logger> s_EngineLogger;
	};

}