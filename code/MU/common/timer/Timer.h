/*
 * @file Timer.h
 * @brief Timer using timerfd.
 * 
 * @version 1.0
 * @date Thu Jun 28 09:39:22 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author shiwei<shiwei2012@gmail.com>
 */

#ifndef Timer_H_
#define Timer_H_

#include "comm/Agent.h"
#include "comm/Epoll.h"
#include "comm/EpollEvent.h"

#include <sys/timerfd.h>

class Timer : public Agent
{
    public:
        virtual ~Timer();

        Timer(Epoll *pEpoll);

        /**
         * @brief Create a new timer.
         *
         * @return  0 if success, or -1
         */
        int create();

        /**
         * @brief Stop the timer.
         *
         * @return 0 if a success, or -1
         */
        int disarm();

        /**
         * @brief Set expiration time of this timer.
         *
         * @param initial Initial expiration time, 
         *          relative to current time in seconds.
         * @param interval Specify the period in seconds for repeated timer,
         *          if this parameter is 0, 
         *          the timer is set to expire just once.
         *
         * @return 0 if a success, or -1
         */
        int setTime(int initial, int interval);

        /**
         * @brief Called by Epoll when timer expired.
         *
         * @return 0 if a success, or -1
         */
        int recvData();

        /**
         * @brief callback for expiration
         *
         * @param times times of expiration
         *
         * @return 0 if a success, or -1
         */
        virtual int expirationCallback(uint64_t times);

    protected:
        int m_TimerFd;
        struct itimerspec m_TimerSpec;
        EpollEvent m_EpollEvent;
};


#endif  // Timer_H_

