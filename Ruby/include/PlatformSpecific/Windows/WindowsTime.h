#pragma once

#include "Ruby/Main/Time.h"

namespace Ruby {


	static double s_Freq = 0.0;

	namespace Time {

		TimeStruct getCurrentSystemTime()
		{
			SYSTEMTIME systime;
			GetSystemTime(&systime);
			return *(TimeStruct*)&systime;
		}

		void init()
		{
			LARGE_INTEGER li;
			RB_ASSERT(QueryPerformanceFrequency(&li), "QueryPerformanceFrequency failed!\n");
			s_Freq = 1.0 / (double)li.QuadPart;
		}

	}

	static inline const char* timingPrecisionToStr(TimingPrecision tp)
	{
		switch (tp)
		{
		case TimingPrecision::Nanoseconds:  return "Nanoseconds";
		case TimingPrecision::Microseconds: return "Microseconds";
		case TimingPrecision::Milliseconds: return "Milliseconds";
		case TimingPrecision::Seconds:      return "Seconds";
		}
	}

	DeltaTime::DeltaTime(TimingPrecision precision)
	{
		m_Factor = (double)precision * s_Freq;
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		m_PreviousTime = time.QuadPart;
	}

	double DeltaTime::getAndClock()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		double delta = (double)(time.QuadPart - m_PreviousTime) * m_Factor;
		m_PreviousTime = time.QuadPart;
		return delta;
	}

	ScopedTimer::ScopedTimer(const char* name, TimingPrecision precision)
		: m_Name(name), m_Str(timingPrecisionToStr(precision))
	{
		m_Factor = (double)precision * s_Freq;
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		m_Start = time.QuadPart;
	}

	ScopedTimer::~ScopedTimer()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		double delta = (double)(time.QuadPart - m_Start) * m_Factor;
		RB_INFO("Timing %s took %.7f %s", m_Name, delta, m_Str);
	}

}