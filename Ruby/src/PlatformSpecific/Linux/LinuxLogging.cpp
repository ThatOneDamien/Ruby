#include "ruby_pch.h"

#include "Ruby/Main/Core.h"

#include "Ruby/Main/Logging.h"

namespace Ruby {

	static uint16_t s_DefaultColor;
	SharedPtr<Logger> Logger::s_EngineLogger;
	SharedPtr<Logger> Logger::s_ClientLogger;

	void Logger::init()
	{
	
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
	}

	void Logger::internalSetLogColor(LogColor textColor, LogColor highlightColor)
	{
	}

	void Logger::internalSetLogColor(uint16_t color)
	{
	}

	void Logger::internalResetLogColor()
	{
	}
}
