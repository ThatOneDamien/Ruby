#include "ruby_pch.h"

#include "Ruby/Main/Core.h"

#include "Ruby/Main/Time.h"

namespace Ruby {


	static double s_Freq = 0.0;

	namespace Time {

		TimeStruct getSystemTime()
		{
			SYSTEMTIME systime;
			GetSystemTime(&systime);
			return *(TimeStruct*)&systime;
		}

		TimeStruct getLocalTime()
		{
			SYSTEMTIME systime;
			GetLocalTime(&systime);
			return *(TimeStruct*)&systime;
		}

		void init()
		{
			LARGE_INTEGER li;
			RB_ASSERT(QueryPerformanceFrequency(&li), "QueryPerformanceFrequency failed!");
			s_Freq = 1.0 / (double)li.QuadPart;
		}

	}

	DeltaTime::DeltaTime()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		m_PreviousTime = time.QuadPart;
	}

	double DeltaTime::getSecondsAndClock()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		double delta = (double)(time.QuadPart - m_PreviousTime) * s_Freq;
		m_PreviousTime = time.QuadPart;
		return delta;
	}

	double DeltaTime::getMillisAndClock()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		double delta = (double)(time.QuadPart - m_PreviousTime) * 1000.0 * s_Freq;
		m_PreviousTime = time.QuadPart;
		return delta;
	}

	double DeltaTime::getMicrosAndClock()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		double delta = (double)(time.QuadPart - m_PreviousTime) * 1000000.0 * s_Freq;
		m_PreviousTime = time.QuadPart;
		return delta;
	}

	double DeltaTime::getNanosAndClock()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		double delta = (double)(time.QuadPart - m_PreviousTime) * 1000000000.0 * s_Freq;
		m_PreviousTime = time.QuadPart;
		return delta;
	}

	ScopedTimer::ScopedTimer(const char* name, ScopedTimerFunc func)
		: m_Name(name), m_Func(func)
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		m_Start = time.QuadPart;
	}

	ScopedTimer::~ScopedTimer()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		double delta = (double)(time.QuadPart - m_Start) * 1000.0 * s_Freq;
		m_Func(delta);
	}

}