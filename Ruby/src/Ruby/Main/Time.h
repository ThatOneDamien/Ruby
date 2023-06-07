#pragma once


namespace Ruby {

	enum class TimingPrecision : uint32_t
	{
		Nanoseconds    = 1000000000,
		Microseconds   = 1000000,
		Milliseconds   = 1000,
		Seconds        = 1
	};

	struct TimeStruct {

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

		// Similar but not equivalent to Windows API SYSTEMTIME struct.

		TimeStruct getSystemTime();
		TimeStruct getLocalTime();

		void init();

	}

	class DeltaTime
	{
	public:
		DeltaTime(TimingPrecision precision);
		~DeltaTime() = default;

		double getAndClock();

	private:
		double m_Factor;
		int64_t m_PreviousTime;

	};

	class ScopedTimer
	{
	public:
		ScopedTimer(const char* name, TimingPrecision precision);
		~ScopedTimer();

	private:
		double m_Factor;
		int64_t m_Start;
		const char* m_Name, *m_Str;

	};

}