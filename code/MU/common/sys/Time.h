/*
 * =====================================================================================
 *
 *       Filename:  Timer.h
 *
 *    Description:  对时间操作的封装
 *
 *        Version:  1.0
 *        Created:  04/25/2011 09:19:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#ifndef COMMON_SYS_TIME_H_
#define COMMON_SYS_TIME_H_

#include <string>

#include <sys/time.h>
#include <sys/types.h>


/**
 * @brief 封装对时间的操作
 */
class Time
{
public:

    Time();

    Time(int64_t);

    enum Clock { Realtime, Monotonic };

    /** 
     * @brief 获取当前时间
     * 
     * @param clock 
     *      - Realtime: 系统rtc时间
     *      - Monotonic: 系统从boost起经过的时间
     * 
     * @return  当前时间
     */
    static Time now(Clock clock= Realtime);

    /** 
     * @brief 构造一个时间对象
     * 
     * @param usec: 构造的时间(秒)
     * 
     * @return 
     */
    static Time seconds(int64_t usec);

    /** 
     * @brief 构造一个时间对象 
     * 
     * @param milli 构造的进间(毫秒)
     * 
     * @return 
     */
    static Time milliSeconds(int64_t milli);

    /** 
     * @brief 构造一个时间对象 
     * 
     * @param micro 构造的时间(微秒)
     * 
     * @return 
     */
    static Time microSeconds(int64_t micro);
    
    /** 
     * @brief 将Time转换成timeval结构
     * 
     * @return 
     */
    operator timeval() const;

    /** 
     * @brief 将时间转换成秒
     * 
     * @return 
     */
    int64_t toSeconds() const;

    /** 
     * @brief 将时间转换成毫秒
     * 
     * @return 
     */
    int64_t toMilliSeconds() const;

    /** 
     * @brief 将时间转换成微秒
     * 
     * @return 
     */
    int64_t toMicroSeconds() const;

    double toSecondsDouble() const;
    double toMilliSecondsDouble() const;
    double toMicroSecondsDouble() const;

    /** 
     * @brief 将时间转换成字符串,例如: 2009-10-26 10:47:47.932
     * 
     * @return 
     */
    std::string toDateTime() const;

    /** 
     * @brief 将时间转换成字符串,例如: 14543d 02:47:47.932
     * 
     * @return 
     */
    std::string toDuration() const;

    Time operator-() const
    {
        return Time(-_usec);
    }

    Time operator-(const Time& rhs) const
    {
        return Time(_usec - rhs._usec);
    }

    Time operator+(const Time& rhs) const
    {
        return Time(_usec + rhs._usec);
    }

    Time& operator+=(const Time& rhs)
    {
        _usec += rhs._usec;
        return *this;
    }

    Time& operator-=(const Time& rhs)
    {
        _usec -= rhs._usec;
        return *this;
    }

    bool operator<(const Time& rhs) const
    {
        return _usec < rhs._usec;
    }

    bool operator<=(const Time& rhs) const
    {
        return _usec <= rhs._usec;
    }

    bool operator>(const Time& rhs) const
    {
        return _usec > rhs._usec;
    }

    bool operator>=(const Time& rhs) const
    {
        return _usec >= rhs._usec;
    }

    bool operator==(const Time& rhs) const
    {
        return _usec == rhs._usec;
    }

    bool operator!=(const Time& rhs) const
    {
        return _usec != rhs._usec;
    }

    double operator/(const Time& rhs) const
    {
        return (double)_usec / (double)rhs._usec;
    }

    Time& operator*=(int rhs)
    {
        _usec *= rhs;
        return *this;
    }

    Time operator*(int rhs) const
    {
        Time t;
        t._usec = _usec * rhs;
        return t;
    }

    Time& operator/=(int rhs)
    {
        _usec /= rhs;
        return *this;
    }

    Time operator/(int rhs) const
    {
        Time t;
        t._usec = _usec / rhs;
        return t;
    }

    Time& operator*=(int64_t rhs)
    {
        _usec *= rhs;
        return *this;
    }

    Time operator*(int64_t rhs) const
    {
        Time t;
        t._usec = _usec * rhs;
        return t;
    }

    Time& operator/=(int64_t rhs)
    {
        _usec /= rhs;
        return *this;
    }

    Time operator/(int64_t rhs) const
    {
        Time t;
        t._usec = _usec / rhs;
        return t;
    }

    Time& operator*=(double rhs)
    {
        _usec = static_cast<int64_t>(static_cast<double>(_usec) * rhs);
        return *this;
    }

    Time operator*(double rhs) const
    {
        Time t;
        t._usec = static_cast<int64_t>(static_cast<double>(_usec) * rhs);
        return t;
    }

    Time& operator/=(double rhs)
    {
        _usec = static_cast<int64_t>(static_cast<double>(_usec) / rhs);
        return *this;
    }

    Time operator/(double rhs) const
    {
        Time t;
        t._usec = static_cast<int64_t>(static_cast<double>(_usec) / rhs);
        return t;
    }

private:

    int64_t _usec;
};

#endif  // COMMON_SYS_TIME_H_

