#include "ruby_pch.h"

#include "Ruby/Main/Core.h"

#include "Ruby/Main/Time.h"

#include <time.h>

namespace Ruby 
{
    static double s_Freq = 0.0;

    namespace Time 
    {
        TimeStruct getLocalTime()
        {
            TimeStruct res;
            struct timespec ts = {0,0};
            struct tm tm = {};

            clock_gettime(CLOCK_REALTIME, &ts);
            localtime_r(&ts.tv_sec, &tm);


            res.year = (uint16_t)tm.tm_year;
            res.month = (uint16_t)tm.tm_mon;
            res.dayOfMonth = (uint16_t)tm.tm_mday;
            res.dayOfWeek = (uint16_t)tm.tm_wday;
            res.hour = (uint16_t)tm.tm_hour;
            res.minute = (uint16_t)tm.tm_min;
            res.second = (uint16_t)tm.tm_sec;
            res.milliseconds = (uint16_t)(ts.tv_nsec/1000000);

            return res;
        }

        void init() {}
    }

    DeltaTime::DeltaTime()
    {
        timespec time;
        clock_gettime(CLOCK_MONOTONIC_RAW, &time);
        m_PreviousTime = time.tv_sec * 1000000000l + time.tv_nsec;
    }

    double DeltaTime::getSecondsAndClock()
    {
        timespec time;
        clock_gettime(CLOCK_MONOTONIC_RAW, &time);
        long temp = time.tv_sec * 1000000000l + time.tv_nsec;
        double delta = (double)(temp - m_PreviousTime) * 0.000000001;
        m_PreviousTime = temp;
        return delta;
    }

    double DeltaTime::getMillisAndClock()
    {
        timespec time;
        clock_gettime(CLOCK_MONOTONIC_RAW, &time);
        long temp = time.tv_sec * 1000000000l + time.tv_nsec;
        double delta = (double)(temp - m_PreviousTime) * 0.000001;
        m_PreviousTime = temp;
        return delta;
    }

    double DeltaTime::getMicrosAndClock()
    {
        timespec time;
        clock_gettime(CLOCK_MONOTONIC_RAW, &time);
        long temp = time.tv_sec * 1000000000l + time.tv_nsec;
        double delta = (double)(temp - m_PreviousTime) * 0.001;
        m_PreviousTime = temp;
        return delta;
    }

    double DeltaTime::getNanosAndClock()
    {
        timespec time;
        clock_gettime(CLOCK_MONOTONIC_RAW, &time);
        long temp = time.tv_sec * 1000000000l + time.tv_nsec;
        double delta = (double)(temp - m_PreviousTime);
        m_PreviousTime = temp;
        return delta;
    }

    ScopedTimer::ScopedTimer(const char* name, ScopedTimerFunc func)
        : m_Name(name), m_Func(func)
    {
        timespec time;
        clock_gettime(CLOCK_MONOTONIC_RAW, &time);
        m_Start = time.tv_sec * 1000000000l + time.tv_nsec;
    }

    ScopedTimer::~ScopedTimer()
    {
        timespec time;
        clock_gettime(CLOCK_MONOTONIC_RAW, &time);
        long temp = time.tv_sec * 1000000000l + time.tv_nsec;
        m_Func((double)(temp - m_Start) * 0.000001);
    }

}
