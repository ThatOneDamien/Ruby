#include "ruby_pch.h"

#include "Ruby/Main/Core.h"

// NOTE: DO NOT INCLUDE ANYWHERE ELSE IN THE PROGRAM EXCEPT IN
// Logging.cpp, AS THIS FILE CONTAINS SOURCE CODE THAT WOULD RESULT
// IN A MULTIPLE DECLARATIONS ERROR IF DUPLICATED.
#include "Ruby/Main/Logging.h"

namespace Ruby 
{
	static HANDLE s_ConsoleHandle;
	static uint16_t s_DefaultColor;
	SharedPtr<Logger> Logger::s_EngineLogger;
	SharedPtr<Logger> Logger::s_ClientLogger;

	void Logger::init()
	{
		s_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(s_ConsoleHandle, &info);

		// Gets the first byte of data in the wAttributes which holds the color info. 
		// This contains the current text color in the first 4 bits and the highlight color in the last 4.
		s_DefaultColor = (info.wAttributes & 0xFF);
		s_EngineLogger = createShared<Logger>("Engine", LogLevel::Trace);
		s_ClientLogger = createShared<Logger>("Client", LogLevel::Trace);

	}

	Logger::Logger(const char* name, LogLevel logLevel)
		: m_Name(name), m_Level(logLevel), m_CurrentColor(s_DefaultColor), m_SavedColor(s_DefaultColor)
	{}

	void Logger::setLogColor(LogColor textColor, LogColor highlightColor)
	{
		// Prevents both the text and highlight from being the same color, which would render the text invisible.
		if (textColor != highlightColor)
		{
			m_SavedColor = ((uint8_t)highlightColor << 4) | (uint8_t)textColor;
		}
	}

	void Logger::setLogTextColor(LogColor textColor)
	{
		if ((uint8_t)textColor != (m_SavedColor >> 4))
		{
			m_SavedColor = (m_SavedColor & 0xF0) | (uint8_t)textColor;
		}
	}

	void Logger::setLogHighlightColor(LogColor highlightColor)
	{
		if ((uint8_t)highlightColor != (m_SavedColor & 0x0F))
		{
			m_SavedColor = (m_SavedColor & 0x0F) | (uint8_t)highlightColor;
		}
	}

	void Logger::resetDefaultLogColor()
	{
		m_SavedColor = s_DefaultColor;
	}

	void Logger::internalSetLogColor(LogColor textColor, LogColor highlightColor)
	{
		uint16_t color = ((uint8_t)highlightColor << 4) | (uint8_t)textColor;
		if (m_CurrentColor != color)
		{
			SetConsoleTextAttribute(s_ConsoleHandle, color);
			m_CurrentColor = color;
		}
	}

	void Logger::internalSetLogColor(uint16_t color)
	{
		if (m_CurrentColor != color)
		{
			SetConsoleTextAttribute(s_ConsoleHandle, color);
			m_CurrentColor = color;
		}
	}

	void Logger::internalResetLogColor()
	{
		SetConsoleTextAttribute(s_ConsoleHandle, s_DefaultColor);
		m_CurrentColor = s_DefaultColor;
	}
}