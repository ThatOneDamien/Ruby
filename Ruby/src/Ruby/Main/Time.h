#pragma once

typedef void(*ScopedTimerFunc) (double);

namespace Ruby {

	struct TimeStruct 
	{
		uint16_t  year;
		uint16_t  month;
		uint16_t  dayOfWeek;
		uint16_t  dayOfMonth;
		uint16_t  hour;
		uint16_t  minute;
		uint16_t  second;
		uint16_t  milliseconds;
	};

	namespace Time {

		TimeStruct getLocalTime();

		void init();

	}

	class DeltaTime
	{
	public:
		DeltaTime();
		~DeltaTime() = default;

		[[nodiscard]] double getSecondsAndClock();
		[[nodiscard]] double getMillisAndClock();
		[[nodiscard]] double getMicrosAndClock();
		[[nodiscard]] double getNanosAndClock();

	private:
		int64_t m_PreviousTime;
	};

	class ScopedTimer
	{
	public:
		ScopedTimer(const char* name, ScopedTimerFunc func);
		~ScopedTimer();

	private:
		ScopedTimerFunc m_Func;
		int64_t m_Start;
		const char* m_Name;

	};

}