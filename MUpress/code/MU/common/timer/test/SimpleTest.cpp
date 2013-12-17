/*
 * @file SimpleTest.cpp
 * @brief A simple test for timer.
 * 
 * @version 1.0
 * @date Thu Jun 28 11:05:44 2012
 * 
 * @copyright Copyright (C) 2012 UESTC
 * @author lpc<lvpengcheng6300@gmail.com>
 */

#include "comm/comm.h"
#include "timer/timer.h"
#include "DevLog/DevLog.h"

DevLog *g_pDevLog = NULL;
Epoll *g_pEpoll = NULL;

int 
main(int argc, char *argv[])
{
    g_pDevLog = new DevLog();
    g_pEpoll = new Epoll();
    g_pEpoll->initialize(500);

    Timer *pTimer = new Timer(g_pEpoll);
    pTimer->create();
    //pTimer->setTime(10, 0);
    pTimer->setTime(10, 2);
    //pTimer->setTime(0, 2);

    g_pEpoll->run();

    return 0;
}


