#include <sys/epoll.h>
#include "EpollEvent.h"
#include "Epoll.h"
#include "TCPSocket.h"
#include "Error.h"
#include "DevLog/DevLog.h"
extern DevLog *g_pDevLog;

//#define EDGEEVENTS                   (EPOLLET|EPOLLOUT|EPOLLIN | EPOLLERR | EPOLLHUP)
//#define EDGEREVENT                   (EPOLLET|EPOLLIN | EPOLLERR | EPOLLHUP)
#define LEVELREVENT                  EPOLLIN
#define LEVELWEVENT                  EPOLLOUT
#define LEVELEVENTS                  (EPOLLIN |EPOLLOUT| EPOLLERR | EPOLLHUP)
#define EVENTADD                     EPOLL_CTL_ADD
#define EVENTDEL                     EPOLL_CTL_DEL
#define EVENTCHANGE                  EPOLL_CTL_MOD

int EpollEvent::closeRevent( void )
{
    if ( -1 == mFd )
        return SUCCESSFUL;

    if( mEpollEvent.rlock++ )
        return SUCCESSFUL;

    mEpollEvent.read = false;
    mEpollEvent.event = mEpollEvent.event &~LEVELREVENT;
    if(m_Epoll->doEvent(this, mFd,EVENTCHANGE, mEpollEvent.event ) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "EpollEvent::CancelRevent");
        return FAILED;
    }
    return SUCCESSFUL;
}

int EpollEvent::openRevent( void )
{

    if ( -1 == mFd )
    {
        return SUCCESSFUL;
    }

    if( ( mEpollEvent.rlock && --mEpollEvent.rlock )
            || mEpollEvent.read)
        return SUCCESSFUL;

    mEpollEvent.read = true;
    mEpollEvent.event = mEpollEvent.event |LEVELREVENT;
    if( m_Epoll->doEvent(this, mFd,\
                         EVENTCHANGE, mEpollEvent.event ) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "EpollEvent::OpenlRevent");
        return FAILED;
    }

    return SUCCESSFUL;
}

int EpollEvent::closeWevent( void )
{
    if ( -1 == mFd)
        return SUCCESSFUL;

    if( mEpollEvent.wlock++ )
        return SUCCESSFUL;

    mEpollEvent.write = false;
    mEpollEvent.event = mEpollEvent.event &~LEVELWEVENT;
    if(m_Epoll->doEvent(this, mFd,\
                        EVENTCHANGE, mEpollEvent.event ) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "EpollEvent::CancelWevent");
        return FAILED;
    }

    return SUCCESSFUL;
}

int EpollEvent::openWevent( void)
{
    if ( -1 == mFd)
    {
        return SUCCESSFUL;
    }

    if(!(mEpollEvent.write))
    {
        if( (mEpollEvent.wlock && --mEpollEvent.wlock)
                ||mEpollEvent.write )
            return SUCCESSFUL;

        mEpollEvent.write = true;
        mEpollEvent.event = mEpollEvent.event|LEVELWEVENT;
        if( m_Epoll->doEvent(this, mFd,\
                             EVENTCHANGE, mEpollEvent.event) < 0)
        {
            DEV_LOG(LERROR, OUT_BOTH, "EpollEvent::OpenWevent");
            return FAILED;
        }
    }
    return SUCCESSFUL;
}

int EpollEvent::registerREvent(void)
{
    if ( -1 == mFd)
    {
        return SUCCESSFUL;
    }

    mEpollEvent.event = LEVELREVENT;
    mEpollEvent.read = true;
    mEpollEvent.write = false;


    if( m_Epoll->doEvent(this, mFd,\
                         EVENTADD, mEpollEvent.event) < 0 )
    {
        DEV_LOG(LERROR, OUT_BOTH, "EpollEvent::registerREvent");
        return FAILED;
    }

    return SUCCESSFUL;
}

int EpollEvent::registerWEvent(void)
{
    if ( -1 == mFd )
    {
        return SUCCESSFUL;
    }

    mEpollEvent.event = LEVELWEVENT;
    mEpollEvent.read = false;
    mEpollEvent.write = true;


    if( m_Epoll->doEvent(this, mFd,\
                         EVENTADD, mEpollEvent.event) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "EpollEvent::registerWEvent");
        return FAILED;
    }
    return SUCCESSFUL;
}

int EpollEvent::registerRWEvents(void)
{
    if ( -1 == mFd )
    {
        return SUCCESSFUL;
    }

    mEpollEvent.event = LEVELEVENTS;
    mEpollEvent.read = true;
    mEpollEvent.write = true;


    if(m_Epoll->doEvent(this, mFd,\
                        EVENTADD, mEpollEvent.event) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "EpollEvent::registerRWEvent");
        return FAILED;
    }

    return SUCCESSFUL;
}

int EpollEvent::unregisterRWEvents(void)
{
    if ( -1 == mFd)
    {
        return SUCCESSFUL;
    }

    mEpollEvent.event = LEVELEVENTS;
    mEpollEvent.read = false;
    mEpollEvent.write = false;


    if( m_Epoll->doEvent(this, mFd,\
                         EVENTDEL, mEpollEvent.event) < 0)
    {
        DEV_LOG(LERROR, OUT_BOTH, "EpollEvent::UnregisterRWEvent");
        return FAILED;
    }

    return SUCCESSFUL;
}

