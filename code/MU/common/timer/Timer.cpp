/*
 * @file Timer.cpp
 * @brief Timer using timerfd.
 *
 * @version 1.0
 * @date Thu Jun 28 09:56:24 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "Timer.h"

#include "comm/Epoll.h"
#include "comm/EpollEvent.h"

#include "log/log.h"
#include "util/util.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/timerfd.h>

Timer::~Timer()
{
    m_EpollEvent.unregisterRWEvents();

    ::close(m_TimerFd);
}

Timer::Timer(Epoll *pEpoll)
{
    m_iConnect = CONNECTED;
    m_EpollEvent.setEpoll(pEpoll);
    m_TimerFd = 0;
}

int
Timer::create()
{
    m_TimerFd = ::timerfd_create(CLOCK_MONOTONIC, 0);

    if (-1 == m_TimerFd) {
        ERROR_LOG("In Timer::create, timerfd_create() error, %s.",
                  strerror(errno));
        return -1;
    }

    int val = 0;

    if ((val = fcntl(m_TimerFd, F_GETFL, 0)) < 0) {
        ERROR_LOG("In Timer::create, fcntl(F_GETFL) error, %s.",
                strerror(errno));
        return -1;
    }

    val |= O_NONBLOCK;

    if (fcntl(m_TimerFd, F_SETFL, val) < 0) {
        ERROR_LOG("In Timer::create, fcntl(F_SETFL) error, %s.",
                strerror(errno));
        return -1;
    }


    m_EpollEvent.setFd(m_TimerFd);
    m_EpollEvent.setHandler(this);

    if (m_EpollEvent.registerREvent() < 0) {
        ERROR_LOG("In Timer::create, register read event failed.");
        return -1;
    }

    return 0;
}

int
Timer::disarm()
{
    struct itimerspec ts;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = 0;

    int rc = 0;
    rc = ::timerfd_settime(m_TimerFd, 0, &ts, NULL);

    if (-1 == rc) {
        ERROR_LOG("In Timer::disarm, timerfd_settime() error, %s.",
                  strerror(errno));
        return -1;
    }

    return 0;
}

int
Timer::setTime(int initial, int interval)
{
    struct itimerspec ts;

    ts.it_interval.tv_sec = interval;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = initial;
    ts.it_value.tv_nsec = 0;

    int rc = 0;
    rc = ::timerfd_settime(m_TimerFd, 0, &ts, NULL);

    if (-1 == rc) {
        ERROR_LOG("In Timer::setTime, timerfd_settime() error, %s.",
                  strerror(errno));
        return -1;
    }

    return 0;
}

int
Timer::recvData()
{
    uint64_t expirationTimes = 0;

    int rc = 0;

    rc = util::io::readn(m_TimerFd, &expirationTimes, sizeof(expirationTimes));

    if (rc < 0) {
        ERROR_LOG("In Timer::recvData, read timer failed, %s.",
                  strerror(errno));
        return -1;
    }

    // callback

    if (0 == expirationTimes) {
        WARN_LOG("In Timer::recvData, no expirations.");
        return 0;
    }

    if (1 != expirationTimes) {
        WARN_LOG("In Timer::recvData, %ld expirations.", expirationTimes);
    }

    return expirationCallback(expirationTimes);
}

int
Timer::expirationCallback(uint64_t times)
{
    ERROR_LOG("Timer expired.");
    return 0;
}

