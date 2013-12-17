/*
 * =========================================================================
 *
 *       Filename:  client.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012-02-06 01:23:08
 *  Last Modified:  2012-02-06 17:50:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lpc, lvpengcheng6300@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#include "log/log.h"
#include "comm/comm.h"
#include "DevLog/DevLog.h"

#include "ClientAgent.h"
#include "AppProto.h"

DevLog *g_pDevLog = NULL;
//TimerManager *g_pTimerManager = NULL;

int main()
{
    g_pDevLog = new DevLog();
    //g_pTimerManager = new TimerManager();

    Epoll *pEpoll = new Epoll();
    pEpoll->initialize(50);

    UnxDmnSocketAddress oppoAddr("unxdmn.echo");
    ClientAgent *pClientAgent = new ClientAgent(pEpoll, new AppProto(),
            oppoAddr);
    
    pClientAgent->init();
    pClientAgent->connect();

    pEpoll->run();

    return 0;
}
