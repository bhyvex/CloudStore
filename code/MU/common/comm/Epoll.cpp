#include <time.h>
#include <string.h>
#include <errno.h>
#include "Epoll.h"
#include "EpollEvent.h"
#include "TCPAgent.h"
#include "Error.h"
#include "AgentManager.h"
#include "DevLog/DevLog.h"
extern DevLog *g_pDevLog;
#define EPOLL_TIMEOUT_LEN 5
Epoll::Epoll(void):
    mEpollEvents(NULL),
    mEpollFd(-1)
{
//    gettimeofday(&mCurrent, NULL);
}

Epoll::~Epoll(void)
{
    if( mEpollEvents !=NULL )
        delete [] mEpollEvents;
}

int Epoll::getEpollFd()const
{
    return mEpollFd;
}

int Epoll::initialize( unsigned int fdsize)
{
    mEventSize = fdsize;
    mEpollFd = epoll_create( mEventSize );
    if( mEpollFd < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "Epoll:initialize");
        return -1;
    }
    mEpollEvents = new epoll_event[fdsize];
    memset( mEpollEvents, 0, sizeof(epoll_event)*fdsize);
    return 0;
}

const struct timeval& Epoll::getCurrent(void)
{
    return mCurrent;
}

int Epoll::doEvent( EpollEvent* ptr,
                    int fd,
                    int op,
                    unsigned int events)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof( struct epoll_event));
    ev.events = events;
    ev.data.ptr = ptr;
    if( epoll_ctl( mEpollFd, op, fd, &ev) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "Epoll:doEvent");
        return FAILED;
    }
    return SUCCESSFUL;
}


void Epoll::run(void)
{

    int nfds = 0;

    EpollEvent* event = NULL;

    for(;;)
    {

        if( (nfds = epoll_wait( mEpollFd, mEpollEvents,	mEventSize, EPOLL_TIMEOUT_LEN )) <0 )
        {

//			printf("epollfd: %d, mEventsize: %d, timeout:%d\n",mEpollFd, mEventSize, EPOLL_TIMEOUT_LEN);
            if( errno == EINTR )
            {
                continue;
            }
            else
            {
                DEV_LOG(LERROR, OUT_BOTH, "Epoll:epoll_wait");
            }
        }

        if( gettimeofday( &mCurrent, NULL) < 0 )
            DEV_LOG(LERROR, OUT_BOTH, "Epoll:gettimeofday");

//		unsigned long long t = 100000 * mCurrent.tv_sec + mCurrent.tv_usec;
        //	printf("epoll: time: %d\n",t/1000);

        for( int i= 0; i< nfds; i++)
        {

            event = (EpollEvent*)mEpollEvents[i].data.ptr;
            if ( event == NULL )
                continue;
            Agent *agent=event->getHandler();
            if( NULL == agent)
            {
                DEV_LOG(LERROR, OUT_BOTH, "Epoll::agent == NULL");
                continue;
            }
            if ( mEpollEvents[i].events & EPOLLERR
                    || mEpollEvents[i].events & EPOLLHUP )
            {
                if(agent->getState() == CONNECTING)//for the connect
                {
                    if ( agent->getErrno() == EISCONN)
                    {
                        agent->setState(CONNECTED);
                        if(agent->connectAfter(true) < 0)
                        {
                            (AgentManager::getInstance())->recyclerAgent(agent);
                            continue;
                        }
                        agent->resetConnect();
                        continue;
                    }
                    else
                    {
                        if(agent->allowReconnect())
                        {
                            SocketAddress addr;
                            if(SUCCESSFUL == (agent->getOppAddr(addr)))
                            {
                                if( agent->connect(addr) < 0)
                                {
                                }
                            }
                            else
                            {
                                DEV_LOG(LERROR, OUT_BOTH, "OppAddr error");
                                (AgentManager::getInstance())->recyclerAgent(agent);
                            }
                            //if ( agent->connect() < 0) {

                            //}

                            continue;
                        }
                        else
                        {
                            agent->resetConnect();
                            if(agent->connectAfter(false)<0)
                            {
                                DEV_LOG(LERROR, OUT_BOTH, "connect error");
                                (AgentManager::getInstance())->recyclerAgent(agent);
                                continue;
                            }
                            else
                                continue;
                        }
                    }
                }
                else
                {
                    if ( agent->recvData() < 0)
                    {
                        (AgentManager::getInstance())->recyclerAgent(agent);
                        continue;
                    }
                }
            }
            if( mEpollEvents[i].events & EPOLLOUT )
            {
                if(CONNECTED == (agent->getState()))
                {
                    if( agent->sendData() < 0)
                    {
                        (AgentManager::getInstance())->recyclerAgent(agent);
                        continue;
                    }
                }
                else
                {
                    agent->setState(CONNECTED);
                    if(agent->connectAfter(true) < 0)
                    {
                        (AgentManager::getInstance())->recyclerAgent(agent);
                    }
                    continue;
                }
            }
            if( mEpollEvents[i].events & EPOLLIN)
            {
                if( agent->recvData() < 0)
                {
                    (AgentManager::getInstance())->recyclerAgent(agent);
                    continue;
                }
            }
        }
        if(this->doTask() < 0)
        {
            DEV_LOG(LINFO, OUT_BOTH, "Deal With Task Error");
        }
    }
}
