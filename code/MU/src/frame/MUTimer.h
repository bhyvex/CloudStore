/*
 * @file MUTimer.h
 * @brief Timer of MU.
 *
 * @version 1.0
 * @date Thu Jun 28 14:25:59 2012
 *
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#ifndef MUTimer_H_
#define MUTimer_H_

#include "timer/timer.h"

class MUTask;

class MUTimer : public Timer
{
public:
    virtual ~MUTimer();

    explicit MUTimer(Epoll *pEpoll);

    MUTimer(Epoll *pEpoll, MUTask *pTask);

    void setTask(MUTask *pTask);

    /**
     * @brief called when timer expired
     *
     * @param times expirations ocurred
     *
     * @return 0 if a success, or -1
     */
    int expirationCallback(uint64_t times);

private:
    MUTask *m_pTask;
};

#endif  // MUTimer_H_

