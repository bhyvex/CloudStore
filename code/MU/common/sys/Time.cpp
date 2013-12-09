/*
 * =====================================================================================
 *
 *       Filename:  Time.cpp
 *
 *    Description:  时间操作封装
 *
 *        Version:  1.0
 *        Created:  04/26/2011 11:08:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "Time.h"

#include <iomanip>
#include <sstream>
#include <string>

#include <time.h>
#include <sys/types.h>
#include <sys/time.h>

#include "log/log.h"

Time::Time() :
    _usec(0)
{
}

Time::Time(int64_t usec) :
    _usec(usec)
{
}

Time Time::now(Clock clock)
{
    if(clock == Realtime)
    {
        struct timeval tv;
        if(gettimeofday(&tv, 0) < 0)
        {
            ERROR_LOG("Syscall Error: gettimeofday.");
        }
        return Time(tv.tv_sec * static_cast<int64_t>(1000000) + tv.tv_usec);
    }
    else // Monotonic
    {
        struct timespec ts;
        if(clock_gettime(CLOCK_MONOTONIC, &ts) < 0)
        {
            ERROR_LOG("Syscall Error: clock_gettime.");
        }
        return Time(ts.tv_sec * static_cast<int64_t>(1000000) 
                + ts.tv_nsec / static_cast<int64_t>(1000));
    }
}

Time Time::seconds(int64_t t)
{
    return Time(t * static_cast<int64_t>(1000000));
}

Time Time::milliSeconds(int64_t t)
{
    return Time(t * static_cast<int64_t>(1000));
}

Time Time::microSeconds(int64_t t)
{
    return Time(t);
}

Time::operator timeval() const
{
    timeval tv;
    tv.tv_sec = static_cast<time_t>(_usec / 1000000);
    tv.tv_usec = static_cast<long>(_usec % 1000000);
    return tv;
}

int64_t Time::toSeconds() const
{
    return _usec / 1000000;
}

int64_t Time::toMilliSeconds() const
{
    return _usec / 1000;
}

int64_t Time::toMicroSeconds() const
{
    return _usec;
}

double Time::toSecondsDouble() const
{
    return _usec / 1000000.0;
}

double Time::toMilliSecondsDouble() const
{
    return _usec / 1000.0;
}

double Time::toMicroSecondsDouble() const
{
    return static_cast<double>(_usec);
}

std::string Time::toDateTime() const
{
    time_t time = static_cast<time_t>(_usec / 1000000);

    struct tm* t;
    struct tm tr;
    localtime_r(&time, &tr);
    t = &tr;

    char buf[32];
    strftime(buf, sizeof(buf), "%F %H:%M:%S", t);
    //strftime(buf, sizeof(buf), "%x %H:%M:%S", t);

    std::ostringstream os;
    os << buf << ".";
    os.fill('0');
    os.width(3);
    os << static_cast<long>(_usec % 1000000 / 1000);
    return os.str();
}

std::string Time::toDuration() const
{
    int64_t usecs = _usec % 1000000;
    int64_t secs = _usec / 1000000 % 60;
    int64_t mins = _usec / 1000000 / 60 % 60;
    int64_t hours = _usec / 1000000 / 60 / 60 % 24;
    int64_t days = _usec / 1000000 / 60 / 60 / 24;

    using namespace std;

    ostringstream os;
    if(days != 0)
    {
        os << days << "d ";
    }
    os << setfill('0') << setw(2) << hours << ":" << setw(2) << mins << ":" << setw(2) << secs;
    if(usecs != 0)
    {
        os << "." << setw(3) << (usecs / 1000);
    }

    return os.str();
}

