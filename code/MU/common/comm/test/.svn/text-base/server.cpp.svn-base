/*
 * =========================================================================
 *
 *       Filename:  server.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012-02-06 01:23:08
 *  Last Modified:  2012-02-06 01:23:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  NDSL UESTC
 *
 * =========================================================================
 */

#include "log/log.h"
#include "comm/comm.h"
#include "DevLog/DevLog.h"

#include "EchoAgent.h"
#include "AppProto.h"

DevLog *g_pDevLog = NULL;
//TimerManager *g_pTimerManager = NULL;

int main()
{
    g_pDevLog = new DevLog();
    //g_pTimerManager = new TimerManager();

    Epoll *pEpoll = new Epoll();
    pEpoll->initialize(50);

    UnxDmnListenAgent<EchoAgent, AppProto> *pListenAgent = 
        new UnxDmnListenAgent<EchoAgent, AppProto>(pEpoll);

    UnxDmnSocketAddress localAddr("unxdmn.echo");
    
    pListenAgent->init(localAddr, 50);

    pEpoll->run();

    return 0;
}
