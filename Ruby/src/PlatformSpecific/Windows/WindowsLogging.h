#pragma once


// NOTE: DO NOT INCLUDE ANYWHERE ELSE IN THE PROGRAM EXCEPT IN
// Logging.cpp, AS THIS FILE CONTAINS SOURCE CODE THAT WOULD RESULT
// IN A MULTIPLE DECLARATIONS ERROR IF DUPLICATED.
#include "Ruby/Main/Logging.h"

namespace Ruby {

	static HANDLE s_ConsoleHandle;
	static uint16_t s_DefaultColor;
	SharedPtr<Logger> Logger::s_EngineLogger;

	void Logger::init()
	{
		s_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(s_ConsoleHandle, &info);

		// Gets the first byte of data in the wAttributes which holds the color info. 
		// This contains the current text color in the first 4 bits and the highlight color in the last 4.
		s_DefaultColor = (info.wAttributes & 0xFF);
		s_EngineLogger = createShared<Logger>("Engine");
		
	}

	Logger::Logger(const char* prefix, LogLevel logLevel)
		: m_Name(prefix), m_Level(logLevel), m_ConsoleColor(s_DefaultColor)
	{
	}

	void Logger::setLogColor(LogColor textColor, LogColor highlightColor)
	{
		// Prevents both the text and highlight from being the same color, which would render the text invisible.
		if (textColor == highlightColor) return;
		m_ConsoleColor = ((uint8_t)highlightColor << 4) | (uint8_t)textColor;
		SetConsoleTextAttribute(s_ConsoleHandle, m_ConsoleColor);
	}

	void Logger::setLogTextColor(LogColor textColor)
	{
		if ((uint8_t)textColor == (m_ConsoleColor >> 4)) return;
		m_ConsoleColor = (m_ConsoleColor & 0xF0) | (uint8_t)textColor;
		SetConsoleTextAttribute(s_ConsoleHandle, m_ConsoleColor);
	}

	void Logger::setLogHighlightColor(LogColor highlightColor)
	{
		if ((uint8_t)highlightColor == (m_ConsoleColor & 0x0F)) return;
		m_ConsoleColor = (m_ConsoleColor & 0x0F) | (uint8_t)highlightColor;
		SetConsoleTextAttribute(s_ConsoleHandle, m_ConsoleColor);
	}

	void Logger::resetDefaultLogColor()
	{
		m_ConsoleColor = s_DefaultColor;
		SetConsoleTextAttribute(s_ConsoleHandle, m_ConsoleColor);
	}

}